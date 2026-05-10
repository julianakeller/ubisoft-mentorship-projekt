// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCSimulationSubsystem.h"
#include "MentorshipProjekt/GameTime/GameTimeSubsystem.h"

void UNPCSimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// Ensure LogSubsystem is created first
	Collection.InitializeDependency(ULogSubsystem::StaticClass());
	Collection.InitializeDependency(UGameTimeSubsystem::StaticClass());
	
	Super::Initialize(Collection);
	
	TimeSubsystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>();
	
	if (TimeSubsystem)
	{
		TimeSubsystem->OnMinuteChanged.AddDynamic(this, &UNPCSimulationSubsystem::OnMinuteChanged);
		TimeSubsystem->OnHourChanged.AddDynamic(this, &UNPCSimulationSubsystem::OnHourChanged);
		TimeSubsystem->OnDayChanged.AddDynamic(this, &UNPCSimulationSubsystem::OnDayChanged);
		TimeSubsystem->OnWeekChanged.AddDynamic(this, &UNPCSimulationSubsystem::OnWeekChanged);
		TimeSubsystem->OnMonthChanged.AddDynamic(this, &UNPCSimulationSubsystem::OnMonthChanged);
	}
	
	LogSubsystem = GetWorld()->GetSubsystem<ULogSubsystem>();
	if (!LogSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("NPCSimulationSubsystem: LogSubsystem not found during Initialize."));
	}
}

void UNPCSimulationSubsystem::OnMinuteChanged(const FInGameTime& NewTime) {}
void UNPCSimulationSubsystem::OnHourChanged(const FInGameTime& NewTime) {}
void UNPCSimulationSubsystem::OnDayChanged(const FInGameTime& NewTime) {}
void UNPCSimulationSubsystem::OnWeekChanged(const FInGameTime& NewTime) {}
void UNPCSimulationSubsystem::OnMonthChanged(const FInGameTime& NewTime) {}