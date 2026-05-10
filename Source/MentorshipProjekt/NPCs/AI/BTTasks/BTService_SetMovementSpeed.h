// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SetMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UBTService_SetMovementSpeed : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_SetMovementSpeed();

	UPROPERTY(EditAnywhere, Category="Speed")
	float Speed = 200.f;
	
	// Blackboard float key
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector SpeedKey;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
