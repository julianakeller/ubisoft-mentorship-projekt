// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkerRowWidget.h"
#include "WorkerTimelineWidget.h"
#include "ShiftScheduleMenuWidget.h"

void UWorkerRowWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWorkerRowWidget::InitializeRow(const FGuid& InWorkerID, const FText& InName, UShiftScheduleMenuWidget* InMenu)
{
	WorkerID = InWorkerID;
	
	if (WorkerTimeline)
	{
		WorkerTimeline->WorkerID = WorkerID;
		WorkerTimeline->ParentMenu = InMenu;
	}

	if (CharacterNameText)
	{
		CharacterNameText->SetText(InName);
	}
}
