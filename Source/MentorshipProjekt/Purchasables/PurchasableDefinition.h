// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PurchasableDefinition.generated.h"

/**
 * 
 */
class URecipeDefinition;

UCLASS(BlueprintType)
class MENTORSHIPPROJEKT_API UPurchasableDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="General")
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Economy")
	float BasePrice = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Production")
	TObjectPtr<URecipeDefinition> Recipe;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Decay")
	bool bIsPerishable = false;

	// Percent freshness lost per in-game hour
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Decay", meta=(EditCondition="bIsPerishable"))
	float DecayRate = 0.f;

	//Only allows tags under Item.*
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tags", meta=(Categories="Purchasable"))
	FGameplayTagContainer ItemTags;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Purchasable", GetFName());
	}
};
