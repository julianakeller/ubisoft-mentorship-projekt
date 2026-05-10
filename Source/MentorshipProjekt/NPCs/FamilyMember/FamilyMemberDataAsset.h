// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../NPCDataAsset.h"
#include "Engine/DataAsset.h"
#include "MentorshipProjekt/Areas/WorkAreaBase.h"
#include "../Skills/SkillDataAsset.h"
#include "../Skills/SkillProgress.h"
#include "FamilyMemberDataAsset.generated.h"

/**
 * 
 */

class AFamilyMemberCharacter;

USTRUCT(BlueprintType)
struct FWorkPreference
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AWorkAreaBase> PreferredWorkAreaClass;

	// >1 preferred, <1 disliked
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Work Preferences")
	float PreferenceWeight = 1.0f; 
};

UCLASS(BlueprintType)
class MENTORSHIPPROJEKT_API UFamilyMemberDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Work")
	FName FamilyMemberName;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFamilyMemberCharacter> CharacterClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Work")
	float BaseWorkEfficiency = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Work")
	float LearningSpeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TArray<FSkillProgress> StartingSkills;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Work")
	TArray<FWorkPreference> WorkPreferences;
	
	//ToDo Morals
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NPC Stats")
	float MovementSpeed = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTransform SpawnTransform;
	
	UFUNCTION(BlueprintCallable, Category="FamilyMember")
	FPrimaryAssetType GetPrimaryAssetType() const
	{
		return FPrimaryAssetType("FamilyMember");
	}
	
	//Returns nullptr if the work area is not contained in WorkPreferences
	const FWorkPreference* GetWorkPreference(TSubclassOf<AWorkAreaBase> WorkAreaClass) const;
};
