// Fill out your copyright notice in the Description page of Project Settings.


#include "PurchasableInstance.h"
#include "RecipeDefinition.h"

void UPurchasableInstance::Initialize(UPurchasableDefinition* InDefinition, float InQuality, int32 WorkerSkillLevel)
{
	InstanceId = FGuid::NewGuid();
	Definition = InDefinition;
	Quality = InQuality;
	
	Freshness = 1.f;
	
	Quality = CalculateInitialQuality(WorkerSkillLevel);
	
}

float UPurchasableInstance::CalculateInitialQuality(int32 WorkerSkillLevel) const
{
	if (!Definition || !Definition->Recipe)
		return 1.f;

	const int32 RequiredLevel = Definition->Recipe->RequiredSkillLevel;

	if (RequiredLevel <= 0)
		return 1.f;

	float SkillFactor =
		static_cast<float>(WorkerSkillLevel) /
		static_cast<float>(RequiredLevel);

	float RandomFactor = FMath::FRandRange(0.8f, 1.2f);

	return FMath::Clamp(SkillFactor * RandomFactor, 0.5f, 1.5f);
}

float UPurchasableInstance::GetCurrentValue()
{
	return 0.f;  //ToDo
}

void UPurchasableInstance::UpdateFreshness(float CurrentGameTime)
{
	if (!Definition->bIsPerishable)
		return;
	
	float Decay =   Definition->DecayRate; //ToDo will be queried lazily
	Freshness = FMath::Clamp(1.f - Decay, 0.f, 1.f);
}
