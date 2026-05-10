// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskFindRandomLocation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MENTORSHIPPROJEKT_API UBTTaskFindRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskFindRandomLocation();

	UPROPERTY(EditAnywhere, Category="Search")
	float SearchRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector LocationKey;
	
	UPROPERTY(EditAnywhere, Category="Navigation")
	TSubclassOf<UNavigationQueryFilter> NavigationFilterClass;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
