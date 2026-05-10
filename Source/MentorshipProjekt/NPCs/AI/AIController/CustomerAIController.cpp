// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerAIController.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerCharacter.h"

ACustomerAIController::ACustomerAIController()
{
	bAttachToPawn = true;
}

void ACustomerAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void ACustomerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ControlledCharacter = Cast<ACustomerCharacter>(InPawn);
	
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
	
	if (!ControlledCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnPossess: ControlledCharacter not valid."));
		return;
	}
	
	if (ControlledCharacter->BrainComponent)
	{
		if (UCustomerBrainComponent* CustomerBrainComponent = Cast<UCustomerBrainComponent>(ControlledCharacter->BrainComponent))
		{
			ControlledCharacter->BrainComponent->Initialize(ControlledCharacter);
			UE_LOG(LogTemp, Display, TEXT("CustomerAIController: Initialized BrainComponent"));
		
			CustomerBrainComponent->EvaluatePurchase();
		}
	}
}

void ACustomerAIController::OnUnPossess()
{
	Super::OnUnPossess();
}