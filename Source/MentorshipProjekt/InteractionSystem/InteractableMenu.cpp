// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableMenu.h"
#include "MentorshipProjekt/Menu/MenuManager.h"

void AInteractableMenu::HandleInteraction(AActor* Character)
{
	Super::HandleInteraction(Character);
	
	if (!ShiftMenuWidgetClass)
		return;

	if (UMenuManager* MenuManager = GetWorld()->GetSubsystem<UMenuManager>())
	{
		MenuManager->ToggleMenu(ShiftMenuWidgetClass, this);
	}
}

bool AInteractableMenu::CanBeInteracted() const
{
	return InteractionState == EInteractionState::Available
		|| InteractionState == EInteractionState::Interacting;
}

bool AInteractableMenu::IsMenuOpen() const
{
	if (UMenuManager* MenuManager = GetWorld()->GetSubsystem<UMenuManager>())
	{
		return MenuManager->IsMenuOpen();
	}
	return false;
}