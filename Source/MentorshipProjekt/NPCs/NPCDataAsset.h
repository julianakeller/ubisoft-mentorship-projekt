// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NPCDataAsset.generated.h"

/**
 * 
 */

UCLASS(Abstract, BlueprintType)
class MENTORSHIPPROJEKT_API UNPCDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Identity")
	FName DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NPC Stats")
	float MovementSpeed = 1.f;
	
	UFUNCTION(BlueprintCallable, Category="NPC")
	FPrimaryAssetType GetPrimaryAssetType() const
	{
		return FPrimaryAssetType("NPC");
	}
	
	//ToDo group stats into structs later
};
