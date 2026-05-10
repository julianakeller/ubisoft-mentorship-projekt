// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_SafeLatentBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskInteractWithWorkstation.generated.h"

class UInteractableWorkstationComponent;
class IWorkstationInteractionInterface;
class USingleStageProductionComponent;
class APlayableFamilyMemberCharacter;
/**
 * 
 */
UCLASS(Blueprintable)
class MENTORSHIPPROJEKT_API UBTTaskInteractWithWorkstation : public UBTTask_SafeLatentBase
{
	GENERATED_BODY()
	
public:
	UBTTaskInteractWithWorkstation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	
	virtual void CleanupTask() override;
	
private:
	
	// Cached Components
	
	IWorkstationInteractionInterface* CachedProductionInterface = nullptr;
	UPROPERTY()
	UInteractableWorkstationComponent* CachedWorkstationComponent = nullptr;
	
	// Production

	void OnProductionCycleComplete(APlayableFamilyMemberCharacter* Interactor);
	void OnProductionStopped(APlayableFamilyMemberCharacter* Interactor);
	void OnProductionSettingsChanged(UInteractableWorkstationComponent* WSC);
	
	void RemoveDelegateBindings() const;
	
	void ClearCachedState();
};
