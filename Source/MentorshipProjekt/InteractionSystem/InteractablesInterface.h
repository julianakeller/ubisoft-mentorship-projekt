// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractablesInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractablesInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class AActor; //ToDo

class MENTORSHIPPROJEKT_API IInteractablesInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void OnInteractionRangeEntered() = 0;
	virtual void OnInteractionRangeExited() = 0;
	virtual void OnInteracted(AActor* Character) = 0;
	virtual void OnInteractedSecondary(AActor* Character) = 0;
	virtual void HandleInteraction(AActor* Character) = 0;
	virtual void HandleInteractionSecondary(AActor* Character) = 0;
	virtual bool CanBeInteracted() const = 0;
};
