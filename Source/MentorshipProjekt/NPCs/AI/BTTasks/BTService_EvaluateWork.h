// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_EvaluateWork.generated.h"

/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UBTService_EvaluateWork : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_EvaluateWork();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
