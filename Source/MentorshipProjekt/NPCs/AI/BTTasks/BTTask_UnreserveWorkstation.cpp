// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_UnreserveWorkstation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerCharacter.h"

EBTNodeResult::Type UBTTask_UnreserveWorkstation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;

	if (!CachedOwnerComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UBTTask_UnreserveWorkstation: No CachedOwnerComp"));
		return EBTNodeResult::Failed;
	}
	
	CachedController = OwnerComp.GetAIOwner();
	if (!CachedController)
	{
		UE_LOG(LogTemp, Error, TEXT("UBTTask_UnreserveWorkstation: No controller found"));
		return EBTNodeResult::Failed;
	}
	
	CachedCharacter = Cast<ANPCCharacter>(CachedController->GetPawn());
	if (!CachedCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UBTTask_UnreserveWorkstation: No Character found"));
		return EBTNodeResult::Failed;
	}
	
	CachedBrain = CachedCharacter->BrainComponent;
	
	if (!CachedBrain)
	{
		UE_LOG(LogTemp, Error, TEXT("UBTTask_UnreserveWorkstation: No Brain found"));
		return EBTNodeResult::Failed;
	}
	
	UReservationComponentBase* Reservation = CachedBrain->ReservedWorkstation;
	
	if (!Reservation)
	{
		return EBTNodeResult::Succeeded;
	}
	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	BB->SetValueAsBool("HasReservedWorkstation", false);
	
	Reservation->ClearReservation();
	
	CachedBrain->ReservedWorkstation = nullptr;
	
	UE_LOG(LogTemp, Display, TEXT("UBTTask_UnreserveWorkstation: Succeeded"));
	
	return EBTNodeResult::Succeeded;
}
