// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ReserveWorkstation.generated.h"

class ANPCCharacter;
class UNPCBrainComponent;
class UCustomerBrainComponent;
class ACustomerCharacter;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UBTTask_ReserveWorkstation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;

	UPROPERTY()
	UNPCBrainComponent* CachedBrain = nullptr;
	
	UPROPERTY()
	AAIController* CachedController;
	
	UPROPERTY()
	ANPCCharacter* CachedCharacter;
};
