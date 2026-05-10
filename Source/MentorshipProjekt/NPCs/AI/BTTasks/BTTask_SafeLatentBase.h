// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SafeLatentBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MENTORSHIPPROJEKT_API UBTTask_SafeLatentBase : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTask_SafeLatentBase();

protected:
	
	// Timeout
	
	UPROPERTY(EditAnywhere, Category="Safety")
	float MaxTaskDuration = 120.f;
	
	UPROPERTY(EditAnywhere, Category="Safety")
	bool bUseTimeout = true;
	
	void StartTimeout(UBehaviorTreeComponent& OwnerComp);

	FTimerHandle TimeoutHandle;

	UFUNCTION()
	void OnTaskTimeout();
	
	virtual void FinishTaskSafe(EBTNodeResult::Type Result);
	
	bool bTaskFinished = false;
	
	virtual void CleanupTask();

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory) override;
	
	// Cached Components
	
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;
	
};
