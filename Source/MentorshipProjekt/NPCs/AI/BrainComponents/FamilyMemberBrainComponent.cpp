// Fill out your copyright notice in the Description page of Project Settings.


#include "FamilyMemberBrainComponent.h"

#include "Components/ArrowComponent.h"
#include "MentorshipProjekt/GameTime/GameTimeSubsystem.h"
#include "Shifts/ShiftManager.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"
#include "MentorshipProjekt/Areas/WorkAreaBase.h"
#include "MentorshipProjekt/Areas/WorkAreaManager.h"
#include "MentorshipProjekt/InteractionSystem/InteractableComponents/FamilyMemberReservationComponent.h"
#include "MentorshipProjekt/InteractionSystem/InteractableComponents/WorkstationInteractionInterface.h"

UFamilyMemberBrainComponent::UFamilyMemberBrainComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UFamilyMemberBrainComponent::Initialize(AFamilyMemberCharacter* InOwner)
{
	Super::Initialize(InOwner);
}

AInteractableBase* UFamilyMemberBrainComponent::GetTargetWorkstation()
{
	return CurrentDecision.TargetWorkstation;
}

void UFamilyMemberBrainComponent::BeginPlay()
{
	Super::BeginPlay();
}

FWorkDecision UFamilyMemberBrainComponent::EvaluateWork()
{
	CurrentDecision = FWorkDecision();
	
	if (!ShiftManager)
	{
		ShiftManager = GetWorld()->GetSubsystem<UShiftManager>();
	}
	if (!GameTimeSubsystem)
	{
		GameTimeSubsystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>();
	}
	
	if (!OwnerCharacter || !ShiftManager || !GameTimeSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("FamilyMemberBrainComponent: OwnerCharacter couldn't be found."));
		return CurrentDecision;
	}
	
	AFamilyMemberCharacter* FamilyMemberCharacter = Cast<AFamilyMemberCharacter>(OwnerCharacter);
	if (!FamilyMemberCharacter)
	{
		return CurrentDecision;
	}

	// Get current shift:
	const int32 CurrentHour = GameTimeSubsystem->GetCurrentTime().Hour;
	FShiftData* CurrentShift = ShiftManager->GetCurrentShift(FamilyMemberCharacter->FamilyMemberId, CurrentHour);
	
	if (!CurrentShift || CurrentShift->AssignedArea.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("FamilyMemberBrainComponent: Current shift or assigned shift area not found."));
		return CurrentDecision;
	}

	// Find work area:
	AWorkAreaBase* CurrentWorkArea = ResolveWorkArea(CurrentShift->AssignedArea);
	if (!CurrentWorkArea)
	{
		UE_LOG(LogTemp, Error, TEXT("FamilyMemberBrainComponent: No work area found."));
		return CurrentDecision;
	}

	CurrentDecision.bHasActiveShift = true;
	CurrentDecision.AssignedArea = CurrentShift->AssignedArea;
	
	// Find best workstation (any with ProductionComponent):
	AInteractableBase* BestStation = FindBestWorkstation(CurrentWorkArea);
	
	if (!BestStation)
	{
		UE_LOG(LogTemp, Error, TEXT("FamilyMemberBrainComponent: No best workstation found."));
		return CurrentDecision;
	}

	CurrentDecision.bHasValidTask = true;
	CurrentDecision.WorkArea = CurrentWorkArea;
	CurrentDecision.TargetWorkstation = BestStation;
	CurrentDecision.CurrentInterface = FindWorkstationInteractionInterface(BestStation);
	
	UFamilyMemberReservationComponent* Reservation = BestStation->FindComponentByClass<UFamilyMemberReservationComponent>();

	if (Reservation && Reservation->FamilyMemberInteractionPoint)
	{
		CurrentDecision.TargetInteractionPoint = Reservation->FamilyMemberInteractionPoint->GetComponentLocation();
	}
	else
	{
		// Fallback to workstation location
		CurrentDecision.TargetInteractionPoint = BestStation->GetActorLocation();
	}
	
	return CurrentDecision;
}

AWorkAreaBase* UFamilyMemberBrainComponent::ResolveWorkArea(FName AreaName)
{
	if (!WorkAreaManager)
		WorkAreaManager = GetWorld()->GetSubsystem<UWorkAreaManager>();
	if (!WorkAreaManager)
	{
		UE_LOG(LogTemp, Error, TEXT("FamilyMemberBrainComponent: WorkAreaManager not found."));
		return nullptr;
	}
	return const_cast<AWorkAreaBase*>(WorkAreaManager->GetWorkAreaByName(AreaName));
}

AInteractableBase* UFamilyMemberBrainComponent::FindBestWorkstation(AWorkAreaBase* Area)
{
	AInteractableBase* Best = nullptr;
	int32 HighestPriority = -1;

	if (!Area)
	{
		return nullptr;
	}

	for (AInteractableBase* WorkStation : Area->WorkStations)
	{
		if (!WorkStation)
		{
			continue;
		}

		IWorkstationInteractionInterface* Interface = FindWorkstationInteractionInterface(WorkStation);

		if (APlayableFamilyMemberCharacter* FamilyMemberCharacter = Cast<APlayableFamilyMemberCharacter>(OwnerCharacter))
		{
			if (!Interface || !Interface->NeedsAttention(FamilyMemberCharacter))
			{
				continue;
			}
		}
		

		int32 Priority = Interface->GetPriority();
		if (Priority > HighestPriority)
		{
			HighestPriority = Priority;
			Best = WorkStation;
		}
	}

	return Best;
}

IWorkstationInteractionInterface* UFamilyMemberBrainComponent::FindWorkstationInteractionInterface(
	AInteractableBase* Workstation)
{
	// Find whichever production/interaction component is present
	for (UActorComponent* Comp : Workstation->GetComponents())
	{
		IWorkstationInteractionInterface* Interface = Cast<IWorkstationInteractionInterface>(Comp);
		if (Interface)
		{
			return Interface;
		}
	}
	return nullptr;
}
