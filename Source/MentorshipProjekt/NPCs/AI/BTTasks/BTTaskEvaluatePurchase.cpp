// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskEvaluatePurchase.h"

#include "MentorshipProjekt/NPCs/Customer/CustomerCharacter.h"
#include "AIController.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskEvaluatePurchase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Cache references:
	
	CachedOwnerComp = &OwnerComp;

	if (!CachedOwnerComp)
	{
		return EBTNodeResult::Failed;
	}
	
	CachedController = OwnerComp.GetAIOwner();
	if (!CachedController)
	{
		UE_LOG(LogTemp, Error, TEXT("BTTask_EvaluatePurchase: No controller found"));
		return EBTNodeResult::Failed;
	}
	
	CachedCharacter = Cast<ACustomerCharacter>(CachedController->GetPawn());
	if (!CachedCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("BTTask_EvaluatePurchase: No Character found"));
		return EBTNodeResult::Failed;
	}
	
	CachedBrain = Cast<UCustomerBrainComponent>(CachedCharacter->BrainComponent);
	
	if (!CachedBrain)
	{
		UE_LOG(LogTemp, Error, TEXT("BTTask_EvaluatePurchase: No Brain found"));
		return EBTNodeResult::Failed;
	}
	const FCustomerDecision& Decision = CachedBrain->EvaluatePurchase();

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("bHasValidPurchase: %s"), Decision.bHasValidPurchase ? TEXT("true") : TEXT("false"));

	UE_LOG(LogTemp, Warning, TEXT("bReservedWorkstation: %s"), Decision.bReservedWorkstation ? TEXT("true") : TEXT("false"));

	UE_LOG(LogTemp, Warning, TEXT("DesiredQuantity: %d"), Decision.DesiredQuanity);

	UE_LOG(LogTemp, Warning, TEXT("DesiredItem: %s"), Decision.DesiredItem ? *Decision.DesiredItem->GetName() : TEXT("nullptr"));

	UE_LOG(LogTemp, Warning, TEXT("TargetWorkstation: %s"), Decision.TargetWorkstation ? *Decision.TargetWorkstation->GetName() : TEXT("nullptr"));

	UE_LOG(LogTemp, Warning, TEXT("MatchedTags Num: %d"), Decision.MatchedTags.Num());

	for (const FGameplayTag& Tag : Decision.MatchedTags)
	{
		UE_LOG(LogTemp, Warning, TEXT("MatchedTag: %s"), *Tag.ToString());
	}

	BB->SetValueAsBool("HasValidPurchase", Decision.bHasValidPurchase);
	BB->SetValueAsObject("TargetWorkstation", Decision.TargetWorkstation);
	BB->SetValueAsVector("TargetInteractionPoint", Decision.TargetInteractionPoint);
	BB->SetValueAsObject("DesiredItem", Decision.DesiredItem);
	BB->SetValueAsInt("DesiredQuantity", Decision.DesiredQuanity);
	BB->SetValueAsBool("HasReservedWorkstation", Decision.bReservedWorkstation);

	return EBTNodeResult::Succeeded;
}

