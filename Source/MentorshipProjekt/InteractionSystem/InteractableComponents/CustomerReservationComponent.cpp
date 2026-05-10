// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerReservationComponent.h"

void UCustomerReservationComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UArrowComponent*> Components;
	GetOwner()->GetComponents<UArrowComponent>(Components);

	for (UArrowComponent* Component : Components)
	{
		if (Component &&
			Component->GetName().StartsWith(TEXT("CustomerInteractionPoint")))
		{
			CustomerInteractionPoint = Component;
		}
	}
}