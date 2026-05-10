// Fill out your copyright notice in the Description page of Project Settings.


#include "ReservationComponentBase.h"

bool UReservationComponentBase::TryReserve(ANPCCharacter* NPC)
{
	if (!NPC) return false;

	if (!ReservedBy)
	{
		ReservedBy = NPC;
		OnReservationStarted.Broadcast(NPC);
		return true;
	}

	return ReservedBy == NPC;
}

void UReservationComponentBase::ClearReservation()
{
	if (ReservedBy)
	{
		ReservedBy = nullptr;
		OnReservationEnded.Broadcast();
	}
}

bool UReservationComponentBase::IsReservedBy(ANPCCharacter* NPC) const
{
	return ReservedBy == NPC;
}

bool UReservationComponentBase::IsReserved() const
{
	return ReservedBy != nullptr;
}
