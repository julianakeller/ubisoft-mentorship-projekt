// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_SafeLatentBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BuyItem.generated.h"

class APlayableFamilyMemberCharacter;
class UCustomerBrainComponent;
class UTendingWorkstationComponent;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UBTTask_BuyItem : public UBTTask_SafeLatentBase
{
	GENERATED_BODY()
	
public:
	UBTTask_BuyItem();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void CleanupTask() override;
	
private:

	UPROPERTY()
	UTendingWorkstationComponent* CachedWorkstation = nullptr;

	UPROPERTY()
	UCustomerBrainComponent* CachedBrain = nullptr;

	void OnTransactionComplete(bool bSuccess);
	void OnOrderFailed(APlayableFamilyMemberCharacter* Worker);
	
	void ClearCachedState();
};
