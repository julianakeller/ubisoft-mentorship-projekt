// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BuyItem.h"

#include "AIController.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"
#include "MentorshipProjekt/InteractionSystem/InteractableComponents/TendingWorkstationComponent.h"
#include "MentorshipProjekt/NPCs/AI/BrainComponents/CustomerBrainComponent.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerCharacter.h"


UBTTask_BuyItem::UBTTask_BuyItem()
{
	NodeName = "Buy Item";
	
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true; // required for delegates
}

EBTNodeResult::Type UBTTask_BuyItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bTaskFinished = false;
	CleanupTask();
	
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	ACustomerCharacter* Customer = Cast<ACustomerCharacter>(Controller->GetPawn());
	if (!Customer) return EBTNodeResult::Failed;

	CachedBrain = Customer->FindComponentByClass<UCustomerBrainComponent>();
	if (!CachedBrain) return EBTNodeResult::Failed;

	AInteractableBase* Target = CachedBrain->GetTargetWorkstation();
	if (!Target) return EBTNodeResult::Failed;

	CachedWorkstation = Target->FindComponentByClass<UTendingWorkstationComponent>();
	if (!CachedWorkstation) return EBTNodeResult::Failed;

	UPurchasableDefinition* Item = CachedBrain->GetCurrentDecision().DesiredItem;
	if (!Item) return EBTNodeResult::Failed;
	
	int32 Amount = CachedBrain->GetCurrentDecision().DesiredQuanity;
	if (Amount <= 0) return EBTNodeResult::Failed;

	CachedWorkstation->OnTransactionComplete.AddUObject(this, &UBTTask_BuyItem::OnTransactionComplete);
	CachedWorkstation->GetOnWorkStopped().AddUObject(this, &UBTTask_BuyItem::OnOrderFailed);
	
	StartTimeout(OwnerComp);
	
	const bool bStarted = CachedWorkstation->TryPurchaseCustomer(Customer, Item, Amount);

	if (!bStarted)
	{
		FinishTaskSafe(EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_BuyItem::ClearCachedState()
{
	CleanupTask();
}

void UBTTask_BuyItem::CleanupTask()
{
	if (CachedWorkstation)
	{
		CachedWorkstation->OnTransactionComplete.RemoveAll(this);
		CachedWorkstation->GetOnWorkStopped().RemoveAll(this);
	}
	CachedWorkstation = nullptr;
	CachedBrain = nullptr;
}

void UBTTask_BuyItem::OnTransactionComplete(bool bSuccess)
{
	UE_LOG(LogTemp, Display, TEXT("BTTask Transaction complete."));
	if (CachedBrain)
	{
		if (bSuccess)
		{
			CachedBrain->OnOrderServed();
		}
		CachedBrain->EvaluateStay();
	}
	
	FinishTaskSafe(bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

void UBTTask_BuyItem::OnOrderFailed(APlayableFamilyMemberCharacter* Worker)
{
	FinishTaskSafe(EBTNodeResult::Failed);
}
