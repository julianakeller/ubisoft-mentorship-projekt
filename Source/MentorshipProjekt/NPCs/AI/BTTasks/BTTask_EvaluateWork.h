// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EvaluateWork.generated.h"

class AFamilyMemberCharacter;
class UFamilyMemberBrainComponent;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UBTTask_EvaluateWork : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;

	UPROPERTY()
	UFamilyMemberBrainComponent* CachedBrain = nullptr;
	
	UPROPERTY()
	AAIController* CachedController;
	
	UPROPERTY()
	AFamilyMemberCharacter* CachedCharacter;
};
