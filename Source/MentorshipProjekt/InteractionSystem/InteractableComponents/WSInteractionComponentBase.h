// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "InteractableWorkstationComponent.h"
#include "FamilyMemberReservationComponent.h"
#include "MentorshipProjekt/GameTime/GameTimeSubsystem.h"
#include "MentorshipProjekt/Purchasables/PurchasableManagerSubsystem.h"
#include "WorkstationInteractionInterface.h"
#include "MentorshipProjekt/NPCs/FamilyMember/PlayableFamilyMemberCharacter.h"
#include "WSInteractionComponentBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MENTORSHIPPROJEKT_API UWSInteractionComponentBase : public UInteractableComponentBase, public IWorkstationInteractionInterface
{
	GENERATED_BODY()
	
public:
	
	virtual FOnWorkstationStateChanged& GetOnWorkComplete() override { return OnWorkCompleteDelegate; }
	virtual FOnWorkstationStateChanged& GetOnWorkStopped() override { return OnWorkStoppedDelegate; }
	
	virtual int32 GetPriority() const override;
	//override StartWork -> reservation, set current setting, set recipe
	
	bool UseTaskTimeout() const override;
	
	virtual bool StartWork(APlayableFamilyMemberCharacter* Worker) override;
	virtual void StopWork() override;
	virtual bool NeedsAttention(APlayableFamilyMemberCharacter* AttentionFromCharacter) override;
	virtual bool NeedsAttention() override;
	virtual bool SetNextProductionSettings() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stages")
	bool bWorkerIsActing = false;

	UPROPERTY()
	APlayableFamilyMemberCharacter* ProducingCharacter = nullptr;
	
	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		if (!ProductionWorkstation)
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerProductionComponent: WSInteractionComponentBase requires a InteractableWorkstationComponent on the Interactable."));
		}
	}
	
protected:

	FOnWorkstationStateChanged OnWorkCompleteDelegate;
	FOnWorkstationStateChanged OnWorkStoppedDelegate;
	
	// Current workstation setting being produced
	const FWorkstationProductionSetting* CurrentSetting = nullptr;
	
	UPROPERTY()
	URecipeDefinition* Recipe = nullptr;
	
	int32 GetProducerSkillLevel();
	
	float ActionStartTime = 0.f;
	float ActionDuration = 0.f;
	
	void ShowProgressWidget();
	void HideProgressWidget();
	void UpdateProgressWidget(float Progress);
	virtual void UpdateProgressTick() override;
	virtual float UpdateProgress() override;
	
	// Producing Character Animation Helpers:
	void SetCharacterActionTag(EActionType Tag);
	void ClearCharacterActionTag();
};
