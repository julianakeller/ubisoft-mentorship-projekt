// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableWidgetTextComponent.h"
#include "InteractableBase.h"
#include "InteractionWidget.h"
#include "Components/WidgetComponent.h"

void UInteractableWidgetTextComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (CachedInteractable->InteractionWidgetComponent)
	{
		InteractionWidgetReference = Cast<UInteractionWidget>(CachedInteractable->InteractionWidgetComponent->GetWidget());
	}
	
	UpdateText();
	UpdateWidgetVisibility();
}

void UInteractableWidgetTextComponent::OnRangeEntered(AActor* Interactor)
{
	UpdateWidgetVisibility();
}

void UInteractableWidgetTextComponent::OnRangeExited(AActor* Interactor)
{
	UpdateWidgetVisibility();
}

void UInteractableWidgetTextComponent::OnInteract(AActor* Interactor)
{
	UpdateText();
}

void UInteractableWidgetTextComponent::UpdateWidgetVisibility()
{
	//Should only be visible if player, not NPC, is near
	
	if (!CachedInteractable->InteractionWidgetComponent)
	{
		return;
	}

	const bool bShouldBeVisible = CachedInteractable->bPlayerInRange && (CachedInteractable->InteractionState == EInteractionState::Available);
	CachedInteractable->InteractionWidgetComponent->SetHiddenInGame(!bShouldBeVisible, true);
}

void UInteractableWidgetTextComponent::UpdateText()
{
	if (!CachedInteractable || !InteractionWidgetReference)
	{
		return;
	}

	const bool bIsInteracting = CachedInteractable->GetInteractionState() == EInteractionState::Interacting;

	InteractionWidgetReference->PopulateRows(InteractionEntries, bIsInteracting);
	
	UpdateWidgetVisibility();
}