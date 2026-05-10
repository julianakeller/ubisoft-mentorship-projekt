// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FamilyMemberDataAsset.h"
#include "../NPCInstanceData.h"
#include "../Skills/SkillDataAsset.h"
#include "FamilyInstanceData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FFamilyInstanceData : public FNPCInstanceData
{
	GENERATED_BODY()

	//Dynamic family member stats
	
public:
	
	TSubclassOf<AWorkAreaBase> CurrentWorkArea = nullptr;
	
	//Maps SkillNames to the skill's progress
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<FGameplayTag, FSkillProgress> AcquiredSkills;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Data")
	const UFamilyMemberDataAsset* FamilyMemberData = nullptr;
	
	// Adds Delta to Energy
	void UpdateEnergy(float Delta)
	{
		Energy += Delta;
		UpdateHappiness();
	}
	
	// Adds Delta to Recreation
	void UpdateRecreation(float Delta)
	{
		Recreation += Delta;
		UpdateHappiness();
	}
	
	// Adds Delta to Social
	void UpdateSocial(float Delta)
	{
		Social += Delta;
		UpdateHappiness();
	}
	
	float GetEnergy() const
	{
		return Energy;
	}
	
	float GetRecreation() const
	{
		return Recreation;
	}
	
	float GetSocial() const
	{
		return Social;
	}
	
	UPROPERTY()
	bool bHighHappiness = false;

	UPROPERTY()
	bool bLowHappiness = false;

	UPROPERTY()
	bool bHighLoyalty = false;

	UPROPERTY()
	bool bLowLoyalty = false;
	
	UPROPERTY()
	bool bLoggedLoyalty = false;
	
	UPROPERTY()
	bool bLoggedHappiness = false;
	
	void InitializeFromDataAsset(const UFamilyMemberDataAsset* DataAsset)
	{
		if (!DataAsset)
			return;

		//Add starting skill from data asset:
		AcquiredSkills.Empty();
		
		for (const FSkillProgress& SkillProgress : DataAsset->StartingSkills)
		{
			if (!SkillProgress.Skill)
			{
				UE_LOG(LogTemp, Warning, TEXT("FamilyInstanceData InitializeFromDataAsset: Skill pointer is null in FamilyMemberDataAsset %s"), *DataAsset->GetName());
				continue;
			}
			AcquiredSkills.Add(SkillProgress.Skill->SkillName, SkillProgress);
		}
		
		FamilyMemberData = DataAsset;
	}
	
	/*void AddXP(FName SkillName, float XPAmount, float LearningMultiplier = 1.f)
	{
		if (XPAmount <= 0.f)
		{
			return;
		}

		FSkillProgress* SkillProgress = AcquiredSkills.Find(SkillName);

		if (!SkillProgress)
		{
			return;
		}
		
		SkillProgress->CurrentXP += XPAmount * LearningMultiplier * FamilyMemberData->LearningSpeedMultiplier;

		HandleLevelUp(SkillProgress);
	}*/ 
	
protected:
	
	UPROPERTY()
	float LongTermHappiness = 50.f;
	
	//Need for sleep/rest
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float Energy = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float Social = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float Recreation = 50.f;
	
	//Influenced directly by wellbeing (GetOverallWellbeing)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Emotion")
	float Happiness = 50.f;

	//Influenced over time by happiness
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Emotion")
	float Loyalty = 50.f;
	
	static constexpr float HighHappinessThreshold = 85.f;
	static constexpr float LowHappinessThreshold = 15.f;

	static constexpr float HighLoyaltyThreshold = 85.f;
	static constexpr float LowLoyaltyThreshold = 15.f;
	
	void ClampNeeds()
	{
		Energy = FMath::Clamp(Energy, 0.f, 100.f);
		Social = FMath::Clamp(Social, 0.f, 100.f);
		Recreation = FMath::Clamp(Recreation, 0.f, 100.f);
		Happiness = FMath::Clamp(Happiness, 0.f, 100.f);
		Loyalty = FMath::Clamp(Loyalty, 0.f, 100.f);
	}

	float GetOverallWellbeing()
	{
		ClampNeeds();
		return (Energy + Social + Recreation) / 3.f;
	}
	
	void UpdateHappiness()
	{
		//constexpr float WellbeingInfluenceRate = 0.1f;
		constexpr float WellbeingInfluenceRate = 1.f; // ToDo Testing
		constexpr float WorkPreferenceInfluenceRate = 1.f;
		
		const float Wellbeing = GetOverallWellbeing();
		
		Happiness += (Wellbeing - Happiness) * WellbeingInfluenceRate;
		
		// Work preference influence:
		if (FamilyMemberData && CurrentWorkArea)
		{
			const FWorkPreference* CurrentWorkAreaPreference = FamilyMemberData->GetWorkPreference(CurrentWorkArea->GetClass());

			const float WorkModifier = CurrentWorkAreaPreference? CurrentWorkAreaPreference->PreferenceWeight : 0.f;
			
			Happiness += WorkModifier * WorkPreferenceInfluenceRate;
		}
		
		Happiness = FMath::Clamp(Happiness, 0.f, 100.f);
		Happiness >= HighHappinessThreshold ? bHighHappiness = true : bHighHappiness = false;
		Happiness <= LowHappinessThreshold ? bLowHappiness = true :	bLowHappiness = false;
		UpdateLoyaltyFromHappiness();
	}

	void UpdateLoyaltyFromHappiness(float InfluenceRate = 0.05f)
	{
		constexpr float NeutralHappiness = 50.f;

		const float TrendDelta = LongTermHappiness - NeutralHappiness;

		Loyalty += TrendDelta * InfluenceRate;

		Loyalty = FMath::Clamp(Loyalty, 0.f, 100.f);
		
		Loyalty >= HighLoyaltyThreshold ? bHighLoyalty = true : bHighLoyalty = false;
		Loyalty <= LowLoyaltyThreshold ? bLowLoyalty = true :	bLowLoyalty = false;
	}
	
	void UpdateLongTermHappiness()
	{
		constexpr float SmoothingRate = 0.02f;
		LongTermHappiness += (Happiness - LongTermHappiness) * SmoothingRate;
	}
	
	/*void HandleLevelUp(FSkillProgress* SkillProgress)
	{
		if (!SkillProgress)
		{
			return;
		}
		
		while (true)
		{
			const float RequiredXP = SkillProgress->Skill->XPRequiredBase * FMath::Pow(SkillProgress->Skill->XPScalingMultiplier, SkillProgress->CurrentLevel);
			if (SkillProgress->CurrentXP >= RequiredXP)
			{
				SkillProgress->CurrentXP -= RequiredXP;
				SkillProgress->CurrentLevel++;
			}
			else
			{
				break;
			}
		}
	}*/
};
