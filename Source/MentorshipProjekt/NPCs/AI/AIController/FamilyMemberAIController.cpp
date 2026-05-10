// Fill out your copyright notice in the Description page of Project Settings.


#include "FamilyMemberAIController.h"

#include "MentorshipProjekt/NPCs/FamilyMember/FamilyMemberCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MentorshipProjekt/NPCs/AI/BrainComponents/FamilyMemberBrainComponent.h"

AFamilyMemberAIController::AFamilyMemberAIController()
{
	bAttachToPawn = true;
}

void AFamilyMemberAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ControlledCharacter = Cast<AFamilyMemberCharacter>(InPawn);
	
	if (!ControlledCharacter || !ControlledCharacter->FamilyMemberId.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnPossess: FamilyMemberId not valid yet, skipping setup."));
		return;
	}
	
	if (ControlledCharacter->BrainComponent)
	{
		ControlledCharacter->BrainComponent->Initialize(ControlledCharacter);
		UE_LOG(LogTemp, Display, TEXT("CustomerAIController: Initialized BrainComponent"));
	}
	
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void AFamilyMemberAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AFamilyMemberAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}
