// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FamilyEconomySubsystem.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FEconomySnapshot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float Income = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float Expenses = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float ReputationChange = 0.f;
};

class UGameTimeSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoldChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReputationChanged);

UCLASS()
class MENTORSHIPPROJEKT_API UFamilyEconomySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable)
	void AddGold(float Amount);
	
	UFUNCTION(BlueprintPure)
	float GetGold() const { return Gold; }
	
	UFUNCTION(BlueprintCallable)
	void AddReputation(float Amount);

	UFUNCTION(BlueprintPure)
	float GetReputation() const { return Reputation; }
	
	UPROPERTY(BlueprintAssignable)
	FOnGoldChanged OnGoldChanged;

	UPROPERTY(BlueprintAssignable)
	FOnReputationChanged OnReputationChanged;
	
	// Statistics:
	
	UFUNCTION(BlueprintPure)
	const TArray<FEconomySnapshot>& GetHourlyStats() const { return HourlyStats; }

	UFUNCTION(BlueprintPure)
	const TArray<FEconomySnapshot>& GetDailyStats() const { return DailyStats; }

private:

	UPROPERTY()
	float Gold = 0.f;

	UPROPERTY()
	float Reputation = 0.f;
	
	UPROPERTY()
	TObjectPtr<UGameTimeSubsystem> TimeSubsystem;

	UFUNCTION()
	void HandleHourChanged(const FInGameTime& NewTime);

	UFUNCTION()
	void HandleDayChanged(const FInGameTime& NewTime);
	
	void FinalizeHour();
	void FinalizeDay();
	
	// Current accumulation:
	float CurrentHourIncome = 0.f;
	float CurrentHourExpenses = 0.f;
	float CurrentHourReputation = 0.f;
	
	// Historical data:
	UPROPERTY()
	TArray<FEconomySnapshot> HourlyStats;

	UPROPERTY()
	TArray<FEconomySnapshot> DailyStats;
	
	template<typename T>
	void TrimArray(TArray<T>& Array, int32 MaxSize);
	
	UPROPERTY(EditDefaultsOnly, Category="Retention")
	int32 MaxHourlyEntries = 168; // >= 24

	UPROPERTY(EditDefaultsOnly, Category="Retention")
	int32 MaxDailyEntries = 60; // >= 30
};
