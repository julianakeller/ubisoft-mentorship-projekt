// Fill out your copyright notice in the Description page of Project Settings.


#include "FamilyMemberReservationComponent.h"

#include "Components/ArrowComponent.h"

void UFamilyMemberReservationComponent::OnInteract(AActor* Interactor)
{
	ANPCCharacter* Character = Cast<ANPCCharacter>(Interactor);
	if (Character)
	{
		TryReserve(Character);
	}
}

void UFamilyMemberReservationComponent::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<UArrowComponent*> Components;
	GetOwner()->GetComponents<UArrowComponent>(Components);

	for (UArrowComponent* Component : Components)
	{
		if (Component &&
			Component->GetName().StartsWith(TEXT("FamilyMemberInteractionPoint")))
		{
			FamilyMemberInteractionPoint = Component;
		}
	}
}
