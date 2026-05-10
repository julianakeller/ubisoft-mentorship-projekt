// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DecreaseBlackboardInt.generated.h"

/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UBTTask_DecreaseBlackboardInt : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_DecreaseBlackboardInt();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	// Blackboard key selectable in BT editor
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector FloatKey;

	// Amount to subtract
	UPROPERTY(EditAnywhere, Category="Config")
	float DecreaseAmount = 1;
};
