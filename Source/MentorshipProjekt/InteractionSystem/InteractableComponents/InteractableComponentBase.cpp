// Fill out your copyright notice in the Description page of Project Settings.
#include "InteractableComponentBase.h"

#include "CustomerReservationComponent.h"
#include "InteractableWorkstationComponent.h"
#include "SingleStageProductionComponent.h"
#include "FamilyMemberReservationComponent.h"
#include "InteractableWorkstationComponent.h"
#include "../InteractableBase.h"

UInteractableComponentBase::UInteractableComponentBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UInteractableComponentBase::BeginPlay()
{
	Super::BeginPlay();
	
	GameTimeSubsystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>();
	PurchasableManager = GetWorld()->GetSubsystem<UPurchasableManagerSubsystem>();

	//Get InteractableBase that this component is attached to:
	CachedInteractable = Cast<AInteractableBase>(GetOwner());
	
	if (CachedInteractable)
	{
		ProductionWorkstation = CachedInteractable->FindComponentByClass<UInteractableWorkstationComponent>();
		FamilyMemberReservationComponent = CachedInteractable->FindComponentByClass<UFamilyMemberReservationComponent>();
		CustomerReservationComponent = CachedInteractable->FindComponentByClass<UCustomerReservationComponent>();
		ProductionComponent = CachedInteractable->FindComponentByClass<UWSInteractionComponentBase>();
		
		//Subscribe to InteractableBase events:
		CachedInteractable->OnInteractEvent.AddDynamic(this, &UInteractableComponentBase::HandleInteract);
		CachedInteractable->OnInteractSecondaryEvent.AddDynamic(this, &UInteractableComponentBase::HandleInteractSecondary);
		CachedInteractable->OnRangeEnteredEvent.AddDynamic(this, &UInteractableComponentBase::HandleRangeEntered);
		CachedInteractable->OnRangeExitedEvent.AddDynamic(this, &UInteractableComponentBase::HandleRangeExited);
	}
}

void UInteractableComponentBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Unsubscribe from InteractableBase events:
	if (CachedInteractable)
	{
		CachedInteractable->OnInteractEvent.RemoveDynamic(this, &UInteractableComponentBase::HandleInteract);
		CachedInteractable->OnRangeEnteredEvent.RemoveDynamic(this, &UInteractableComponentBase::HandleRangeEntered);
		CachedInteractable->OnRangeExitedEvent.RemoveDynamic(this, &UInteractableComponentBase::HandleRangeExited);
	}

	Super::EndPlay(EndPlayReason);
}

void UInteractableComponentBase::HandleInteract(AActor* Interactor)
{
	OnInteract(Interactor);
}

void UInteractableComponentBase::HandleInteractSecondary(AActor* Interactor)
{
	OnInteractSecondary(Interactor);
}

void UInteractableComponentBase::HandleRangeEntered(AActor* Interactor)
{
	OnRangeEntered(Interactor);
}

void UInteractableComponentBase::HandleRangeExited(AActor* Interactor)
{
	OnRangeExited(Interactor);
}