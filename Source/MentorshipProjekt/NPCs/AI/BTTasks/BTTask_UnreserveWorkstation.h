// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UnreserveWorkstation.generated.h"

class ANPCCharacter;
class UNPCBrainComponent;
class ACustomerCharacter;
class UCustomerBrainComponent;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UBTTask_UnreserveWorkstation : public UBTTaskNode
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
