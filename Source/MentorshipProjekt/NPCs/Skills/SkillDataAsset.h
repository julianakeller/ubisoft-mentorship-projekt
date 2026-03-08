// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillDataAsset.generated.h"

/**
 * 
 */

class USkillDataAsset;

USTRUCT(BlueprintType)
struct FSkillProgress
{
	GENERATED_BODY()

	UPROPERTY()
	//TObjectPtr used for UCLASS/USTRUCT member variables -> better garbage collection & debugging
	TObjectPtr<USkillDataAsset> Skill;

	UPROPERTY()
	int32 CurrentLevel = 0;

	UPROPERTY()
	float CurrentXP = 0.f;
};

UCLASS(BlueprintType)
class MENTORSHIPPROJEKT_API USkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Skill")
	FName SkillName;
	
	//How long the family member will be gone while acquiring the skill at aschool/academy
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Skill")
	float AcquiringSkillDuration = 5.f; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Skill")
	float XPRequiredBase = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Skill")
	float XPScalingMultiplier = 1.5f;
	
	//ToDo How skill levels affect purchasable quality/value
};
