// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DespawnCustomer.h"

#include "AIController.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerCharacter.h"
#include "MentorshipProjekt/NPCs/Simulation/CustomerSimulationSubsystem.h"
#include "MentorshipProjekt/NPCs/Simulation/CustomerSimulationSubsystem.h"

EBTNodeResult::Type UBTTask_DespawnCustomer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();

	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = Controller->GetPawn();

	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}
	
	ACustomerCharacter* Customer = Cast<ACustomerCharacter>(Pawn);

	if (Customer)
	{
		if (UCustomerSimulationSubsystem* CustomerSim = GetWorld()->GetSubsystem<UCustomerSimulationSubsystem>())
		{
			CustomerSim->DespawnCustomer(Customer->CustomerId);
		}
	}
	
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	
	if (UBrainComponent* Brain = Controller->GetBrainComponent())
	{
		Brain->StopLogic(TEXT("Customer despawn"));
	}

	Controller->UnPossess();

	Pawn->Destroy();

	return EBTNodeResult::Succeeded;
}
