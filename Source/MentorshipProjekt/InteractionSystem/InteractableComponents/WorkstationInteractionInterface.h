// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WorkstationInteractionInterface.generated.h"

class APlayableFamilyMemberCharacter;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWorkstationStateChanged, APlayableFamilyMemberCharacter*);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWorkstationInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MENTORSHIPPROJEKT_API IWorkstationInteractionInterface
{
	GENERATED_BODY()

public:
	// Called by BT task to start interaction
	virtual bool StartWork(APlayableFamilyMemberCharacter* Worker) = 0;

	// Called by BT task AbortTask or when shift ends
	virtual void StopWork() = 0;

	// BrainComponent uses this to decide if workstation needs attention from calling character
	virtual bool NeedsAttention(APlayableFamilyMemberCharacter* AttentionFromCharacter) = 0;
	virtual bool NeedsAttention() = 0;

	// BrainComponent uses this to prioritize between workstations
	virtual int32 GetPriority() const = 0;
	
	virtual bool UseTaskTimeout() const = 0;
	
	virtual bool SetNextProductionSettings() = 0;
	
	// BT task subscribes to this to know when to finish
	virtual FOnWorkstationStateChanged& GetOnWorkComplete() = 0;

	// BT task subscribes to this to know when work was interrupted
	virtual FOnWorkstationStateChanged& GetOnWorkStopped() = 0;
	
protected:
	
	virtual void UpdateProgressTick() = 0;
	
	virtual float UpdateProgress() = 0;
};
