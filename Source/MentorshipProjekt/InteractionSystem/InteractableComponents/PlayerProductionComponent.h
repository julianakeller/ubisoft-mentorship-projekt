// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "MentorshipProjekt/Purchasables/PurchasableManagerSubsystem.h"
#include "PlayerProductionComponent.generated.h"

class APlayableFamilyMemberCharacter;
class USingleStageProductionComponent;
class ATopDownCharacter;
struct FWorkstationProductionSetting;
class UGameTimeSubsystem;
class UInteractableWorkstationComponent;

/**
 * 
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UPlayerProductionComponent : public UInteractableComponentBase
{
	GENERATED_BODY()
	
public:
	UPlayerProductionComponent();
	
protected:
	
	virtual void BeginPlay() override;
	
	//Triggered by Q (secondary interaction button)
	virtual void OnInteractSecondary(AActor* Interactor) override;
	
private:
	
	void HandleProductionFinished(APlayableFamilyMemberCharacter* Interactor);
	
	bool ShouldContinueProducing() const;
	
};
