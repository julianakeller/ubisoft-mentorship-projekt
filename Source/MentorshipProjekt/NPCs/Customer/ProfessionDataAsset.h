// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProfessionDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FProfessionLevelData
{
	GENERATED_BODY()

public:

	//Name of this job level, e.g. Town Merchant vs International Trader
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LevelName;
	
	// Promotion chance per month at this level (0–1)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PromotionChance = 0.f;

	// Income multiplier applied at this level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float IncomeMultiplier = 1.f;
};

UCLASS(BlueprintType)
class MENTORSHIPPROJEKT_API UProfessionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="General")
	FName ProfessionName;

	// Base daily income
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Economy")
	float BaseDailyIncome = 10.f;
	
	// Job levels
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Progression")
	TArray<FProfessionLevelData> Levels;

	// Risk of losing job per month
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Risk")
	float JobLossChance = 0.f;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Profession", GetFName());
	}
	
	const FProfessionLevelData* GetLevelData(int32 Level) const
	{
		if (Levels.IsValidIndex(Level))
		{
			return &Levels[Level];
		}
		return nullptr;
	}
};
