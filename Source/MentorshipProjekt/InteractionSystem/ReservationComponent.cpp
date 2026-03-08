// Fill out your copyright notice in the Description page of Project Settings.


#include "ReservationComponent.h"

void UReservationComponent::OnInteract(AActor* Interactor)
{
	TryReserve(Interactor);
}

bool UReservationComponent::TryReserve(AActor* Actor)
{
	if (!ReservedBy)
	{
		ReservedBy = Actor;
		return true;
	}
	return false;
}

void UReservationComponent::ClearReservation()
{
	ReservedBy = nullptr;
}

bool UReservationComponent::IsReserved() const
{
	return ReservedBy != nullptr;
}