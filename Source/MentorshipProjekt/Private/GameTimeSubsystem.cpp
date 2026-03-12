// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimeSubsystem.h"
#include "Engine/World.h"

void UGameTimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	//UE_LOG(LogTemp, Warning, TEXT("GameTimeSubsystem Initialized"));
}

void UGameTimeSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

TStatId UGameTimeSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UGameTimeSubsystem, STATGROUP_Tickables);
}

void UGameTimeSubsystem::Tick(float DeltaTime)
{
	switch (CurrentTimeSpeed)
	{
	case ETimeSpeed::Paused:
		bIsPaused = true;
		break;
	case ETimeSpeed::Normal:
		bIsPaused = false;
		TimeScale = 60.f;
		break;
	case ETimeSpeed::Fast:
		bIsPaused = false;
		TimeScale = 120.f;
		break;
	case ETimeSpeed::VeryFast:
		bIsPaused = false;
		TimeScale = 180.f;
		break;
	}
	
	if (!bIsPaused)
	{
		AdvanceTime(DeltaTime);
	}
}

void UGameTimeSubsystem::AdvanceTime(float DeltaTime)
{
	float GameSeconds = DeltaTime * TimeScale;
	
	AccumulatedTime += GameSeconds;
	
	int32 MinutesProcessedThisFrame = 0;
	constexpr int32 MaxMinutesPerFrame = 5;
	
	while (AccumulatedTime >= 60.f && MinutesProcessedThisFrame < MaxMinutesPerFrame)
	{
		AccumulatedTime -= 60.f;
		CurrentTime.Minute++;
		
		HandleHourOverflow();
		MinutesProcessedThisFrame++;
		
		OnMinuteChanged.Broadcast(CurrentTime);
	}
}

void UGameTimeSubsystem::HandleHourOverflow()
{
	if (CurrentTime.Minute >= 60)
	{
		CurrentTime.Minute = 0;
		CurrentTime.Hour++;
		OnHourChanged.Broadcast(CurrentTime);
		HandleDayOverflow();
	}
}

void UGameTimeSubsystem::HandleDayOverflow()
{
	if (CurrentTime.Hour >= 24)
	{
		CurrentTime.Hour = 0;
		CurrentTime.Day++;
		OnDayChanged.Broadcast(CurrentTime);
		HandleWeekOverflow();
	}
}

void UGameTimeSubsystem::HandleWeekOverflow()
{
	if (CurrentTime.WeekDay > 7)
	{
		CurrentTime.WeekDay = 1;
		CurrentTime.Week++;
		OnWeekChanged.Broadcast(CurrentTime);
		HandleMonthOverflow();
	}
}

void UGameTimeSubsystem::HandleMonthOverflow()
{
	if (CurrentTime.Week > 4)
	{
		CurrentTime.Week = 1;
		CurrentTime.Month++;
		OnMonthChanged.Broadcast(CurrentTime);
		HandleYearOverflow();
	}
}

void UGameTimeSubsystem::HandleYearOverflow()
{
	if (CurrentTime.Month > 12)
	{
		CurrentTime.Month = 1;
		CurrentTime.Year++;
		OnYearChanged.Broadcast(CurrentTime);
	}
}

void UGameTimeSubsystem::SetTimeScale(float NewScale)
{
	TimeScale = FMath::Max(0.0f, NewScale);
}

float UGameTimeSubsystem::GetTimeScale() const
{
	return TimeScale;
}

const FInGameTime& UGameTimeSubsystem::GetCurrentTime() const
{
	return CurrentTime;
}

float UGameTimeSubsystem::ToSeconds(const float InGameMinutes)
{
	return InGameMinutes * 60.f / TimeScale;
}

void UGameTimeSubsystem::SetPaused(bool bPause)
{
	bIsPaused = bPause;
}

bool UGameTimeSubsystem::IsPaused() const
{
	return bIsPaused;
}