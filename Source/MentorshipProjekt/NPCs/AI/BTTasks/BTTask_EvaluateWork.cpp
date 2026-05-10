// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EvaluateWork.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"
#include "MentorshipProjekt/NPCs/AI/BrainComponents/FamilyMemberBrainComponent.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyMemberCharacter.h"

EBTNodeResult::Type UBTTask_EvaluateWork::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;

	if (!CachedOwnerComp)
	{
		return EBTNodeResult::Failed;
	}
	
	CachedController = OwnerComp.GetAIOwner();
	if (!CachedController)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractWithWorkstation: No controller found"));
		return EBTNodeResult::Failed;
	}
	
	CachedCharacter = Cast<AFamilyMemberCharacter>(CachedController->GetPawn());
	if (!CachedCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractWithWorkstation: No Character found"));
		return EBTNodeResult::Failed;
	}
	
	CachedBrain = Cast<UFamilyMemberBrainComponent>(CachedCharacter->BrainComponent);
	
	if (!CachedBrain)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractWithWorkstation: No Brain found"));
		return EBTNodeResult::Failed;
	}
	
	FWorkDecision Decision = CachedBrain->EvaluateWork();

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}

	BB->SetValueAsBool("HasActiveShift", Decision.bHasActiveShift);
	BB->SetValueAsBool("HasTask", Decision.bHasValidTask);
	BB->SetValueAsName("WorkArea", Decision.AssignedArea);
	BB->SetValueAsObject("TargetWorkstation", Decision.TargetWorkstation);
	BB->SetValueAsVector("TargetInteractionPoint", Decision.TargetInteractionPoint);

	return EBTNodeResult::Succeeded;
}
