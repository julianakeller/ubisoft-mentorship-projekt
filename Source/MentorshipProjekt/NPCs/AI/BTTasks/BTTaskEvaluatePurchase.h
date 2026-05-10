// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskEvaluatePurchase.generated.h"

class ACustomerCharacter;
class UCustomerBrainComponent;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UBTTaskEvaluatePurchase : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;

	UPROPERTY()
	UCustomerBrainComponent* CachedBrain = nullptr;
	
	UPROPERTY()
	AAIController* CachedController;
	
	UPROPERTY()
	ACustomerCharacter* CachedCharacter;
};
