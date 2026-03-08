// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameTimeSubsystem.generated.h"

/**
 * 
 */

enum class ETimeSpeed
{
	Paused,
	Normal,
	Fast,
	VeryFast
};

USTRUCT(BlueprintType)
struct FInGameTime
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Year = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Month = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Week = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Day = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeekDay = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Hour = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Minute = 0;

	FString ToFormattedString() const
	{
		return FString::Printf(TEXT("Day %d - %02d:%02d"), Day, Hour, Minute);
	}
	
	static float TotalMinutes(const FInGameTime& InGameTime)
	{
		int32 TotalMinutes = InGameTime.Day * 24 * 60 + InGameTime.Hour * 60 + InGameTime.Minute;
		return TotalMinutes;
	}
	
	static float DifferenceInMinutes(const FInGameTime& CurrentTime, const FInGameTime& PreviousTime)
	{
		return TotalMinutes(CurrentTime) - TotalMinutes(PreviousTime);
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHourChanged,const FInGameTime&,NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDayChanged,const FInGameTime&,NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeekChanged,const FInGameTime&,NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonthChanged,const FInGameTime&,NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnYearChanged,const FInGameTime&,NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMinuteChanged, const FInGameTime&, NewTime);

UCLASS()
class MENTORSHIPPROJEKT_API UGameTimeSubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
	public:
	
	ETimeSpeed CurrentTimeSpeed = ETimeSpeed::VeryFast;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override {return !bIsPaused;}
	virtual TStatId GetStatId() const override;
	
	//Time Control:
	void SetTimeScale(float NewScale);
	float GetTimeScale() const;
	
	void SetPaused(bool bPause);
	bool IsPaused() const;

	const FInGameTime& GetCurrentTime() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnHourChanged OnHourChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnDayChanged OnDayChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnWeekChanged OnWeekChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnMonthChanged OnMonthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnYearChanged OnYearChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnMinuteChanged OnMinuteChanged;
	
private:
	float AccumulatedTime = 0.f;
	float TimeScale = 60.f; // 1 real time second = 60 in-game seconds
	
	UPROPERTY()
	FInGameTime CurrentTime = FInGameTime();
	
	bool bIsPaused = false;
	
	void AdvanceTime(float DeltaTime);
	void HandleHourOverflow();
	void HandleDayOverflow();
	void HandleWeekOverflow();
	void HandleMonthOverflow();
	void HandleYearOverflow();
};
