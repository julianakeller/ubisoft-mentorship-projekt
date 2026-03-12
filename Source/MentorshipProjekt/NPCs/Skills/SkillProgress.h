#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillDataAsset.h"
#include "SkillProgress.generated.h"

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
	
	void AddXP(float XPAmount, float LearningMultiplier = 1.f)
	{
		if (XPAmount <= 0.f)
		{
			return;
		}
		
		CurrentXP += XPAmount * LearningMultiplier;

		HandleLevelUp();
	}
	
	void HandleLevelUp()
	{
		
		while (true)
		{
			const float RequiredXP = Skill->XPRequiredBase * FMath::Pow(Skill->XPScalingMultiplier, CurrentLevel);
			if (CurrentXP >= RequiredXP)
			{
				CurrentXP -= RequiredXP;
				CurrentLevel++;
			}
			else
			{
				break;
			}
		}
	}
};