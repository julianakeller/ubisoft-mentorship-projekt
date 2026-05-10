// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.h"
#include "FamilyMemberAIController.generated.h"

struct FWorkDecision;
class AFamilyMemberCharacter;
class UBehaviorTree;
class UBlackboardComponent;
class UShiftManager;
struct FShiftData;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API AFamilyMemberAIController : public ANPCAIController
{
	GENERATED_BODY()
	
	//Runs BT
	//Takes BrainComponent's decision and propagates them to Blackboard
	//BT then controls movement, workstation interaction, starting production
	
public:
	
	AFamilyMemberAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	UPROPERTY()
	class UFamilyMemberBrainComponent* Brain;

	// Must be assigned in blueprint
	UPROPERTY(EditDefaultsOnly, Category="AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComp;
	
private:

	// Use to access family member Guid
	UPROPERTY()
	AFamilyMemberCharacter* ControlledCharacter;
};
