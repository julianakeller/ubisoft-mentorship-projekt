// Fill out your copyright notice in the Description page of Project Settings.


#include "LogWindowWidget.h"
#include "Components/VerticalBox.h"
#include "LogEntryWidget.h"
#include "LogSubsystem.h"


void ULogWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ULogSubsystem* LogSubsystem = GetWorld()->GetSubsystem<ULogSubsystem>();
	
	if (LogSubsystem)
	{
		LogSubsystem->OnLogMessageAdded.AddDynamic(
			this,
			&ULogWindowWidget::AddLogMessage
		);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LogWindowWidget: LogSubsystem not found."));
	}
}

void ULogWindowWidget::NativeDestruct()
{
	ULogSubsystem* LogSubsystem = GetWorld()->GetSubsystem<ULogSubsystem>();

	if (LogSubsystem)
	{
		LogSubsystem->OnLogMessageAdded.RemoveDynamic(
			this,
			&ULogWindowWidget::AddLogMessage
		);
	}

	Super::NativeDestruct();
}

void ULogWindowWidget::AddLogMessage(const FLogMessage& Message)
{
	if (!LogContainer || !LogEntryClass)
	{
		UE_LOG(LogTemp, Error, TEXT("LogWindowWidget: LogContainer or LogEntryClass not found."));
		return;
	}
	
	ULogEntryWidget* Entry = CreateWidget<ULogEntryWidget>(
		GetWorld(),
		LogEntryClass
	);

	if (!Entry)
	{
		UE_LOG(LogTemp, Error, TEXT("LogWindowWidget: Could not create LogEntryWidget Entry."));
		return;
	}

	Entry->InitializeLogEntry(Message);

	LogContainer->AddChildToVerticalBox(Entry);
	
}