// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RecipeDefinition.generated.h"

/**
 * 
 */
class UPurchasableDefinition;

USTRUCT(BlueprintType)
struct FRecipeIngredient
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPurchasableDefinition> Item;

	UPROPERTY(EditDefaultsOnly)
	int32 Quantity = 1;
};

UCLASS()
class MENTORSHIPPROJEKT_API URecipeDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName RecipeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRecipeIngredient> Ingredients;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag RequiredSkill; //ToDo Skills class

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RequiredSkillLevel = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag RequiredWorkstationTag; //ToDo
};
