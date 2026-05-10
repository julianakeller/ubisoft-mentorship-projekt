// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerTimelineWidget.h"
#include "ShiftBlockWidget.h"
#include "Components/CanvasPanel.h"
#include "Shifts/ShiftManager.h"
#include "Components/CanvasPanelSlot.h"
#include "ShiftScheduleMenuWidget.h"

float UWorkerTimelineWidget::GetHourFromLocalX(float LocalX) const
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

	ShiftManager = GetWorld()->GetSubsystem<UShiftManager>();
	if (ShiftManager)
	{
		ShiftManager->AddWorkerIfMissing(WorkerID);
	}
	
	bShiftsLoaded = false;
}

FReply UWorkerTimelineWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!ParentMenu)
		return FReply::Unhandled();

	FName SelectedArea = ParentMenu->SelectedWorkArea;
	if (SelectedArea.IsNone())
		return FReply::Unhandled();
	
	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		return FReply::Unhandled();

	const float LocalX = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()).X;
	Width = InGeometry.GetLocalSize().X;
	Height = InGeometry.GetLocalSize().Y;

	const float Hour = GetHourFromLocalX(LocalX);
	
	if (IsOverExistingShift(Hour))
		return FReply::Unhandled();

	bCreatingShift = true;
	CreationStartHour = Hour;

	//For widgets CreateWidget must be used for instantiating
	TempShift = CreateWidget<UShiftBlockWidget>(GetWorld(), ShiftBlockClass);
	TimelineCanvas->AddChild(TempShift);

	TempShift->InitializeShift(Hour, Hour+1, Width, Height, this);

	return FReply::Handled().CaptureMouse(TakeWidget());
}

FReply UWorkerTimelineWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bCreatingShift)
		return FReply::Unhandled();

	const float LocalX = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()).X;
	Width = InGeometry.GetLocalSize().X;
	Height = InGeometry.GetLocalSize().Y;

	//Hour for the current mouse position:
	float CurrentHour = GetHourFromLocalX(LocalX);
	CurrentHour = UShiftBlockWidget::SnapToQuarterHour(CurrentHour);
	
	//Determine start and end of the new shift:
	const float Start = FMath::Min(CreationStartHour, CurrentHour);
	const float End = FMath::Max(CreationStartHour, CurrentHour);
	
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
		FShiftData NewShift;
		NewShift.StartHour = Start;
		NewShift.EndHour = End;
		NewShift.AssignedArea = ParentMenu->SelectedWorkArea;

		FShiftData* NewShiftPtr = ShiftManager->AddShift(WorkerID, NewShift);
		
		if (NewShiftPtr)
		{
			TempShift->SetLinkedShiftData(NewShiftPtr);
			TempShift->InitializeShiftAppearance();
		}
		else
		{
			TimelineCanvas->RemoveChild(TempShift);
			TempShift = nullptr;
		}
	}
	
	return FReply::Handled().ReleaseMouseCapture();
}

void UWorkerTimelineWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bShiftsLoaded)
	{
		Width = MyGeometry.GetLocalSize().X;
		Height = MyGeometry.GetLocalSize().Y;

		if (Width > 0.f)
		{
			bShiftsLoaded = true;
			LoadShiftsFromShiftManager();
		}
	}
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

void UWorkerTimelineWidget::AddShiftBlock(FShiftData& ShiftData)
{
	if (!ShiftBlockClass || !TimelineCanvas)
	{
		return;
	}

	UShiftBlockWidget* ShiftBlock = CreateWidget<UShiftBlockWidget>(GetWorld(), ShiftBlockClass);
	if (!ShiftBlock)
	{
		return;
	}

	TimelineCanvas->AddChild(ShiftBlock);

	ShiftBlock->InitializeShift(ShiftData.StartHour, ShiftData.EndHour, Width, Height, this);
	ShiftBlock->SetLinkedShiftData(&ShiftData);
	ShiftBlock->InitializeShiftAppearance(); // handles color from AssignedArea
}

void UWorkerTimelineWidget::LoadShiftsFromShiftManager()
{
	if (!ShiftManager || !TimelineCanvas)
	{
		return;
	}

	TArray<UWidget*> ToRemove;
	for (UWidget* Child : TimelineCanvas->GetAllChildren())
	{
		if (Cast<UShiftBlockWidget>(Child))
		{
			ToRemove.Add(Child);
		}
	}
	for (UWidget* Widget : ToRemove)
	{
		TimelineCanvas->RemoveChild(Widget);
	}

	FWorkerSchedule* Schedule = ShiftManager->GetWorkerSchedule(WorkerID);
	if (!Schedule)
	{
		return;
	}

	for (FShiftData& Shift : Schedule->Shifts)
	{
		AddShiftBlock(Shift);
	}
}