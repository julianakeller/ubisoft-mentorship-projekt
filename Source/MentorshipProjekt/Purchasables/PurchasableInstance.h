// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MentorshipProjekt/GameTime/GameTimeSubsystem.h"
#include "UObject/Object.h"
#include "PurchasableDefinition.h"
#include "PurchasableInstance.generated.h"

/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UPurchasableInstance : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(UPurchasableDefinition* PurchasableDefinition, float InQuality, int32 WorkerSkillLevel);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid InstanceId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPurchasableDefinition> Definition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Quality = 1.f; //can be above or below 1, 1 being the medium/customer-expected quality

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Freshness = 1.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FInGameTime CreationTime = FInGameTime(0);
	
	UFUNCTION()
	float GetCurrentValue();
	
private:
	void UpdateFreshness(float CurrentGameTime);
	
	float CalculateInitialQuality(int32 WorkerSkillLevel) const;
};
