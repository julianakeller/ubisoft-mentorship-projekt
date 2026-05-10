// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.h"
#include "MentorshipProjekt/NPCs/AI/BrainComponents/CustomerBrainComponent.h"
#include "CustomerAIController.generated.h"

class ACustomerCharacter;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API ACustomerAIController : public ANPCAIController
{
	GENERATED_BODY()
	
public:
	
	ACustomerAIController();
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	UPROPERTY()
	class UCustomerBrainComponent* Brain;

	// Must be assigned in blueprint
	UPROPERTY(EditDefaultsOnly, Category="AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComp;
	
private:
	
	UPROPERTY()
	ACustomerCharacter* ControlledCharacter;
};
