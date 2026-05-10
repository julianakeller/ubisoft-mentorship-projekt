// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ReserveWorkstation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"
#include "MentorshipProjekt/InteractionSystem/InteractableComponents/CustomerReservationComponent.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerCharacter.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyMemberCharacter.h"

EBTNodeResult::Type UBTTask_ReserveWorkstation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	CachedCharacter = Cast<ANPCCharacter>(CachedController->GetPawn());
	if (!CachedCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractWithWorkstation: No Character found"));
		return EBTNodeResult::Failed;
	}
	
	CachedBrain = CachedCharacter->BrainComponent;
	
	if (!CachedBrain)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractWithWorkstation: No Brain found"));
		return EBTNodeResult::Failed;
	}
	
	AInteractableBase* Workstation = CachedBrain->GetTargetWorkstation();

	if (!Workstation)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReserveWorkstation: No workstation cached in brain"));
		return EBTNodeResult::Failed;
	}

	UReservationComponentBase* Reservation = nullptr;

	if (CachedCharacter->IsA(AFamilyMemberCharacter::StaticClass()))
	{
		Reservation = Workstation->FindComponentByClass<UFamilyMemberReservationComponent>();
	}
	else if (CachedCharacter->IsA(ACustomerCharacter::StaticClass()))
	{
		Reservation = Workstation->FindComponentByClass<UCustomerReservationComponent>();
	}
	
	if (!Reservation)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReserveWorkstation: No appropriate reservation component on %s"), *Workstation->GetName());
		return EBTNodeResult::Failed;
	}

	bool bSuccess = Reservation->TryReserve(CachedCharacter);
	
	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReserveWorkstation: Reserved workstation with success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ReserveWorkstation: Reservation unsuccessful"));
	}
	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	BB->SetValueAsBool("HasReservedWorkstation", bSuccess);

	if (bSuccess)
	{
		CachedBrain->SetReservedWorkstation(Reservation);
	}

	return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
