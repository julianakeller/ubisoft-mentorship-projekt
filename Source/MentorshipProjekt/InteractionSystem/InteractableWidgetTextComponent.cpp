// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableWidgetTextComponent.h"
#include "InteractableBase.h"
#include "MentorshipProjekt/Menu/MenuManager.h"

void UInteractableWidgetTextComponent::BeginPlay()
{
	Super::BeginPlay();
	UpdateText();
}

void UInteractableWidgetTextComponent::OnRangeEntered(AActor* Interactor)
{
	UpdateText();
}

void UInteractableWidgetTextComponent::OnRangeExited(AActor* Interactor)
{
	UpdateText();
}

void UInteractableWidgetTextComponent::OnInteract(AActor* Interactor)
{
	UpdateText();
}

void UInteractableWidgetTextComponent::UpdateText() const
{
	if (!CachedInteractable)
		return;

	EInteractionState State = CachedInteractable->GetInteractionState();

	switch (State)
	{
	case EInteractionState::Available:
		CachedInteractable->SetInteractionText(AvailableText);
		break;

	case EInteractionState::Interacting:
		CachedInteractable->SetInteractionText(InteractingText);
		break;

	default:
		CachedInteractable->SetInteractionText(FText::GetEmpty());
		break;
	}
}
