// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "HealthConditionDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MENTORSHIPPROJEKT_API UHealthConditionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="General")
	FName ConditionName;

	// How much this condition reduces max health (0-1)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
	float HealthPenalty = 0.f;

	// Chance per in-game day to improve (0–1)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Progression")
	float DailyRecoveryChance = 0.1f;

	// Chance per in-game day to worsen (0–1)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Progression")
	float DailyWorseningChance = 0.1f;

	// Triggers/Factors that worsen the condition (e.g. alcohol, cold exposure)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Triggers")
	FGameplayTagContainer TriggerTags;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseContractionProbability = 0.01f;
	
	// Triggers/Factors that improve the condition (e.g. warmth, medicine, vegetables)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Triggers")
	FGameplayTagContainer HealingTags;
	
	//Conditions this condition can devleop into if full intensity is reached
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Consequences")
	TArray<UHealthConditionDataAsset*> PossibleConsequences;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Consequences")
	bool bCanLeadToDeath = false;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("HealthCondition", GetFName());
	}
};
