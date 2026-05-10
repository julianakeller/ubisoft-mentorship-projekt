// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "WorkstationInteractionInterface.h"
#include "WSInteractionComponentBase.h"
#include "MentorshipProjekt/NPCs/FamilyMember/PlayableFamilyMemberCharacter.h"
#include "SingleStageProductionComponent.generated.h"

class UFamilyMemberReservationComponent;
class APlayableFamilyMemberCharacter;
class UInteractableWorkstationComponent;
class URecipeDefinition;
class UPurchasableManagerSubsystem;
class UGameTimeSubsystem;
struct FWorkstationProductionSetting;

/**
 * 
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API USingleStageProductionComponent : public UWSInteractionComponentBase
{
	GENERATED_BODY()
	
public:
	
	USingleStageProductionComponent();
	
	// Called by BT task to start interaction
	virtual bool StartWork(APlayableFamilyMemberCharacter* Worker) override;

	// Called by BT task AbortTask or when shift ends
	virtual void StopWork() override;

	// BrainComponent uses this to decide if workstation needs a worker
	virtual bool NeedsAttention() override;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action")
	EActionType ProductionActionTag;
	
private:
	
	void ProduceNextItem();

	void HandleProductionComplete();
	
	FTimerHandle ProductionTimerHandle;
	FTimerHandle ProductionUpdateTimerHandle;
	
};
