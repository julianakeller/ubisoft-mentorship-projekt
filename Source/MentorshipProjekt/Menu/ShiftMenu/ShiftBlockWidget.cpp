// Fill out your copyright notice in the Description page of Project Settings.


#include "ShiftBlockWidget.h"
#include "SlateBasics.h"
#include "SlateCore.h"
#include "WorkAreaDropdownWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "WorkerTimelineWidget.h"
#include "MentorshipProjekt/Areas/WorkAreaBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "MentorshipProjekt/Areas/WorkAreaManager.h"
#include "Shifts/ShiftManager.h"

void UShiftBlockWidget::InitializeShift(const float InStartHour, const float InEndHour, const float InTimelineWidth, const float InTimelineHeight, UWorkerTimelineWidget* InParentTimeline)
{
	StartHour = SnapToQuarterHour(InStartHour);
	EndHour = SnapToQuarterHour(InEndHour);
	TimelineWidth = InTimelineWidth;
	TimelineHeight = InTimelineHeight;
	
	ParentTimeline = InParentTimeline;
	
	SetResizingIndicatorVisibility(false, false);

	UpdateVisual();
}

void UShiftBlockWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetIsFocusable(true);
}

void UShiftBlockWidget::SetStartHour(float NewStart)
{
	if (!ParentTimeline) return;

	const float PrevStart = StartHour;

	NewStart = SnapToQuarterHour(NewStart);
	NewStart = FMath::Clamp(NewStart, 0.f, EndHour - 0.25f);

	if (ParentTimeline->IsShiftOverlapping(NewStart, EndHour, this))
	{
		//Reset to previous start hour if new one is overlapping
		NewStart = PrevStart;
	}

	StartHour = NewStart;
	//Update widget width:
	UpdateVisual();
	
	//Update shift Data:
	if (LinkedShiftData)
	{
		LinkedShiftData->StartHour = StartHour;
	}
}

void UShiftBlockWidget::SetEndHour(float NewEnd)
{
	if (!ParentTimeline) return;

	const float PrevEnd = EndHour;

	NewEnd = SnapToQuarterHour(NewEnd);
	NewEnd = FMath::Clamp(NewEnd, StartHour + 0.25f, 24.f);

	if (ParentTimeline->IsShiftOverlapping(StartHour, NewEnd, this))
	{
		NewEnd = PrevEnd;
	}

	EndHour = NewEnd;
	UpdateVisual();
	
	// Update shift data:
	if (LinkedShiftData)
	{
		LinkedShiftData->EndHour = EndHour;
	}
}

void UShiftBlockWidget::UpdateVisual() const 
{
	//Slot = parent slot
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
	{
		// Calculate position and size of shift block
		float Left = (StartHour / 24.f) * TimelineWidth;
		float Width = ((EndHour - StartHour) / 24.f) * TimelineWidth;

		CanvasSlot->SetPosition(FVector2D(Left, 0.f));
		CanvasSlot->SetSize(FVector2D(Width, TimelineHeight));
	}
	
	//SetRenderTranslation(FVector2D(Left, 0.f));
	//SetDesiredSizeInViewport(FVector2D(Width, TimelineHeight));
}

//NativeOn... function -> receiving input on widget
//FReply -> returned to Slate (Unreal's UI system)
FReply UShiftBlockWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Right click deletes shift
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		DeleteShift();
		return FReply::Handled();
	}
	
	//Only capture LMB clicks
	if (InMouseEvent.GetEffectingButton() != EKeys::LeftMouseButton)
	{
		return FReply::Unhandled(); //Unhandled -> event passed on to parent widgets
	}

	//GetScreenSpacePosition -> mouse position in absolute screen pixels
	//AbsoluteToLocal -> converts absolute position into local (widget) coordinates
	float LocalX = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()).X;
	const float Width = InGeometry.GetLocalSize().X;
	
	const float EdgeDetectionWidth = GetEdgeDetectionWidth(Width);
	
	if (LocalX <= EdgeDetectionWidth)
	{
		InteractionMode = EShiftInteractionMode::ResizingLeft;
		//TakeWidget returns underlying Slate widget
		//CaptureMouse -> continue receiving NativeOnMouse even if cursor leaves widget boudns (until manually stopping capture)
		return FReply::Handled().CaptureMouse(TakeWidget());
	}
	else if (LocalX >= InGeometry.GetLocalSize().X - EdgeDetectionWidth)
	{
		InteractionMode = EShiftInteractionMode::ResizingRight;
		return FReply::Handled().CaptureMouse(TakeWidget());
	}

	return FReply::Handled(); //Don't pass clicks on shifts on to other widgets
}

//Called when mosue moving over widget or anywhere when captured
FReply UShiftBlockWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Show resize indicators if hovering over edges:
	float LocalX = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()).X;
	float Width = InGeometry.GetLocalSize().X;
	UpdateResizingIndicator(LocalX, Width);
	
	//Mouse moving only matters when resizing:
	if (InteractionMode == EShiftInteractionMode::None)
	{
		return FReply::Unhandled();
	}
	
	float DeltaX = InMouseEvent.GetCursorDelta().X; //How many pixels cursor moved since last frame
	float DeltaHours = (DeltaX / TimelineWidth) * 24.f;

	if (InteractionMode == EShiftInteractionMode::ResizingLeft)
		SetStartHour(SnapToQuarterHour(StartHour + DeltaHours));

	if (InteractionMode == EShiftInteractionMode::ResizingRight)
		SetEndHour(SnapToQuarterHour(EndHour + DeltaHours));

	return FReply::Handled();
}

FReply UShiftBlockWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InteractionMode != EShiftInteractionMode::None)
	{
		InteractionMode = EShiftInteractionMode::None;
		return FReply::Handled().ReleaseMouseCapture();
	}

	return FReply::Unhandled();
}

void UShiftBlockWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	SetKeyboardFocus();
}

void UShiftBlockWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetInputMode(FInputModeGameAndUI());
	}
	
	SetResizingIndicatorVisibility(false, false);
}

FReply UShiftBlockWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Delete || InKeyEvent.GetKey() == EKeys::X)
	{
		DeleteShift();

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UShiftBlockWidget::DeleteShift()
{
	// Remove from ShiftManager
	if (ParentTimeline && ParentTimeline->ShiftManager && LinkedShiftData)
	{
		FWorkerSchedule* Schedule = ParentTimeline->ShiftManager->GetWorkerSchedule(
			ParentTimeline->WorkerID);

		if (Schedule)
		{
			int32 ShiftIndex = Schedule->Shifts.IndexOfByKey(*LinkedShiftData);
			if (ShiftIndex != INDEX_NONE)
			{
				ParentTimeline->ShiftManager->RemoveShift(ParentTimeline->WorkerID, ShiftIndex);
			}
		}
	}

	// Remove widget from canvas
	if (ParentTimeline && ParentTimeline->TimelineCanvas)
	{
		ParentTimeline->TimelineCanvas->RemoveChild(this);
	}
}

void UShiftBlockWidget::HandleWorkAreaSelected(FName SelectedArea)
{
	if (LinkedShiftData)
	{
		LinkedShiftData->AssignedArea = SelectedArea;
	}
	
	UpdateShiftAppearance();
}

void UShiftBlockWidget::UpdateShiftAppearance()
{
	
}

void UShiftBlockWidget::InitializeShiftAppearance()
{
	if (!LinkedShiftData)
	{
		return;
	}

	UWorkAreaManager* Manager = GetWorld()->GetSubsystem<UWorkAreaManager>();
	if (!Manager)
	{
		return;
	}

	const AWorkAreaBase* Area = Manager->GetWorkAreaByName(LinkedShiftData->AssignedArea);

	if (!Area)
	{
		return;
	}

	const FLinearColor Color = Area->WorkAreaColor;

	if (ShiftBorder)
	{
		ShiftBorder->SetBrushColor(Color);
	}
}

void UShiftBlockWidget::UpdateResizingIndicator(float LocalX, float Width)
{
	if (!ResizingIndicatorLeft || !ResizingIndicatorRight)
	{
		return;
	}
	
	const float EdgeDetectionWidth = GetEdgeDetectionWidth(Width);

	const bool bLeft = LocalX <= EdgeDetectionWidth;
	const bool bRight = LocalX >= Width - EdgeDetectionWidth;
	
	SetResizingIndicatorVisibility(bLeft, bRight);
}

float UShiftBlockWidget::GetEdgeDetectionWidth(float Width)
{
	return FMath::Clamp(Width * EdgeDetectionRatio,
	8.f,   // minimum usable size
	30.f);   // maximum reasonable size
}

void UShiftBlockWidget::SetResizingIndicatorVisibility(bool bLeft, bool bRight)
{
	if (ResizingIndicatorLeft)
	{
		ResizingIndicatorLeft->SetVisibility(bLeft ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (ResizingIndicatorRight)
	{
		ResizingIndicatorRight->SetVisibility(bRight ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
