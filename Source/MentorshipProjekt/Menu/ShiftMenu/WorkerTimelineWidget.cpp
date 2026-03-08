// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerTimelineWidget.h"
#include "ShiftBlockWidget.h"
#include "Components/CanvasPanel.h"
#include "Shifts/ShiftManager.h"
#include "Components/CanvasPanelSlot.h"

float UWorkerTimelineWidget::GetHourFromLocalX(float LocalX, float Width) const
{
	return FMath::Clamp((LocalX / Width) * 24.f, 0.f, 24.f);
}

bool UWorkerTimelineWidget::IsOverExistingShift(float Hour) const
{
	for (UWidget* Child : TimelineCanvas->GetAllChildren())
	{
		const UShiftBlockWidget* Shift = Cast<UShiftBlockWidget>(Child);
		if (!Shift) continue;

		if (Hour >= Shift->GetStartHour() && Hour <= Shift->GetEndHour())
			return true;
	}
	return false;
}

//NativeConstruct is called when the widget is added to the viewport
void UWorkerTimelineWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (const UGameInstance* GI = GetGameInstance())
	{
		ShiftManager = GI->GetSubsystem<UShiftManager>();
		ShiftManager->AddWorkerIfMissing(WorkerID);
	}
}

FReply UWorkerTimelineWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		return FReply::Unhandled();

	const float LocalX = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()).X;
	const float Width = InGeometry.GetLocalSize().X;

	const float Hour = GetHourFromLocalX(LocalX, Width);
	
	if (IsOverExistingShift(Hour))
		return FReply::Unhandled();

	bCreatingShift = true;
	CreationStartHour = Hour;

	//For widgets CreateWidget must be used for instantiating
	TempShift = CreateWidget<UShiftBlockWidget>(GetWorld(), ShiftBlockClass);
	TimelineCanvas->AddChild(TempShift);

	const float Height = InGeometry.GetLocalSize().Y;
	TempShift->InitializeShift(Hour, Hour+1, Width, Height, this);

	return FReply::Handled().CaptureMouse(TakeWidget());
}

FReply UWorkerTimelineWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bCreatingShift)
		return FReply::Unhandled();

	const float LocalX = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()).X;
	const float Width = InGeometry.GetLocalSize().X;

	//Hour for the current mouse position:
	float CurrentHour = GetHourFromLocalX(LocalX, Width);
	CurrentHour = UShiftBlockWidget::SnapToQuarterHour(CurrentHour);
	
	//Determine start and end of the new shift:
	const float Start = FMath::Min(CreationStartHour, CurrentHour);
	const float End = FMath::Max(CreationStartHour, CurrentHour);
	
	float Height = InGeometry.GetLocalSize().Y;
	TempShift->SetStartHour(Start);
	TempShift->SetEndHour(End);

	return FReply::Handled();
}

FReply UWorkerTimelineWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bCreatingShift)
		return FReply::Unhandled();

	//LogShifts();
	
	bCreatingShift = false;
	
	//Add created shift to ShiftManager:
	float Start = TempShift->GetStartHour();
	float End = TempShift->GetEndHour();
	
	if (ShiftManager)
	{
		FShiftData* NewShiftPtr = ShiftManager->AddNewShift(WorkerID, Start, End);

		if (NewShiftPtr)
		{
			// Save pointer to shift data in newly created shift widget
			TempShift->SetLinkedShiftData(NewShiftPtr);
		}
		else
		{
			//Failed to create/add shift data
			TimelineCanvas->RemoveChild(TempShift);
			TempShift = nullptr;
		}
	}
	
	return FReply::Handled().ReleaseMouseCapture();
}

void UWorkerTimelineWidget::LogShifts() const
{
	for (UWidget* Child : TimelineCanvas->GetAllChildren())
	{
		UShiftBlockWidget* Shift = Cast<UShiftBlockWidget>(Child);
		if (!Shift) continue;
		
		UE_LOG(LogTemp, Display, TEXT("Start: %.2f, End: %.2f"), Shift->GetStartHour(), Shift->GetEndHour());
	}
}

bool UWorkerTimelineWidget::IsShiftOverlapping(const float Start, const float End, const UShiftBlockWidget* IgnoreShift) const
{
	for (UWidget* Child : TimelineCanvas->GetAllChildren())
	{
		const UShiftBlockWidget* Shift = Cast<UShiftBlockWidget>(Child);
		if (!Shift || Shift == IgnoreShift)
		{
			continue;
		}

		// Overlap check:
		if (Start < Shift->GetEndHour() && End > Shift->GetStartHour())
		{
			return true;
		}
	}
	return false;
}