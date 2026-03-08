// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameTimeSubsystem.h"
#include "LoggingSystem/LogSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "NPCSimulationSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UNPCSimulationSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION()
	virtual void OnMinuteChanged(const FInGameTime& NewTime);
	UFUNCTION()
	virtual void OnHourChanged(const FInGameTime& NewTime);
	UFUNCTION()
	virtual void OnDayChanged(const FInGameTime& NewTime);
	UFUNCTION()
	virtual void OnWeekChanged(const FInGameTime& NewTime);
	UFUNCTION()
	virtual void OnMonthChanged(const FInGameTime& NewTime);
	
protected:
	//Last time OnMinuteChanged was executed
	FInGameTime LastUpdateMinute = FInGameTime();
	FInGameTime LastUpdateHour = FInGameTime();
	FInGameTime LastUpdateDay = FInGameTime();
	FInGameTime LastUpdateWeek = FInGameTime();
	FInGameTime LastUpdateMonth = FInGameTime();
	
	UPROPERTY()
	TObjectPtr<ULogSubsystem> LogSubsystem;
	
	UPROPERTY()
	TObjectPtr<UGameTimeSubsystem> TimeSubsystem;
};
