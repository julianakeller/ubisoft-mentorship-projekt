// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkerRowWidget.h"
#include "WorkerTimelineWidget.h"

void UWorkerRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (WorkerTimeline)
	{
		WorkerTimeline->WorkerID = WorkerID;
	}
}