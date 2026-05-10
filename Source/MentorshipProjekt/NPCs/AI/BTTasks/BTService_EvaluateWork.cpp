// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_EvaluateWork.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"
#include "MentorshipProjekt/NPCs/AI/BrainComponents/FamilyMemberBrainComponent.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyMemberCharacter.h"

UBTService_EvaluateWork::UBTService_EvaluateWork()
{
	NodeName = "Evaluate Work Periodically";
	Interval = 60.f;
	RandomDeviation = 5.f;
	bCallTickOnSearchStart = false;
}

void UBTService_EvaluateWork::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return;

	AFamilyMemberCharacter* Character = Cast<AFamilyMemberCharacter>(Controller->GetPawn());
	if (!Character) return;

	UFamilyMemberBrainComponent* Brain = Character->FindComponentByClass<UFamilyMemberBrainComponent>();
	if (!Brain) return;

	FWorkDecision Decision = Brain->EvaluateWork();

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return;
	}

	BB->SetValueAsBool("HasActiveShift", Decision.bHasActiveShift);
	BB->SetValueAsBool("HasTask", Decision.bHasValidTask);
	BB->SetValueAsName("WorkArea", Decision.AssignedArea);
	BB->SetValueAsObject("TargetWorkstation", Decision.TargetWorkstation);
	BB->SetValueAsVector("TargetInteractionPoint", Decision.TargetInteractionPoint);

}
