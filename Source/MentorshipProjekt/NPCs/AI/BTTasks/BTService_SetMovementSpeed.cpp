// Fill out your copyright notice in the Description page of Project Settings.

#include "MentorshipProjekt/NPCs/AI/BTTasks/BTService_SetMovementSpeed.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"

UBTService_SetMovementSpeed::UBTService_SetMovementSpeed()
{
	NodeName = TEXT("Set Movement Speed");

	// Restrict selectable Blackboard keys to float only
	SpeedKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_SetMovementSpeed, SpeedKey));
}

void UBTService_SetMovementSpeed::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AC = OwnerComp.GetAIOwner();
	if (!AC)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(AC->GetPawn());
	if (!Character)
	{
		return;
	}

	UCharacterMovementComponent* MoveComp =
		Character->GetCharacterMovement();

	if (!MoveComp)
	{
		return;
	}

	float DesiredSpeed = Speed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (BB && SpeedKey.SelectedKeyType)
	{
		DesiredSpeed = BB->GetValueAsFloat(SpeedKey.SelectedKeyName);
	}

	MoveComp->MaxWalkSpeed = DesiredSpeed;
}
