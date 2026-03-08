// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCInstanceData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FNPCInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY()
	FGuid NPCID;
	
	//Dynamic NPC stats
};
