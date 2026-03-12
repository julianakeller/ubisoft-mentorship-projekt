// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionMenuComponent.h"
#include "MentorshipProjekt/Menu/MenuManager.h"

void UInteractionMenuComponent::OnInteract(AActor* Interactor)
{
	if (!MenuClass)
		return;

	if (UMenuManager* Manager = GetWorld()->GetSubsystem<UMenuManager>())
	{
		Manager->ToggleMenu(MenuClass, CachedInteractable);
	}
}
