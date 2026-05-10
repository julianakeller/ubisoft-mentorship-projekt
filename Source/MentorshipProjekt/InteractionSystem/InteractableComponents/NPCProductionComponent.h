// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "NPCProductionComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UNPCProductionComponent : public UInteractableComponentBase
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
};
