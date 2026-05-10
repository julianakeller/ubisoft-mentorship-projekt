// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "ReservationComponentBase.generated.h"

class ANPCCharacter;
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnReservationStarted, ANPCCharacter*);
DECLARE_MULTICAST_DELEGATE(FOnReservationEnded);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UReservationComponentBase : public UInteractableComponentBase
{
	GENERATED_BODY()
	
public:
	bool TryReserve(ANPCCharacter* NPC);
	void ClearReservation();
    
	bool IsReservedBy(ANPCCharacter* NPC) const;
	bool IsReserved() const;

	ANPCCharacter* GetReserver() const { return ReservedBy; }

	FOnReservationStarted OnReservationStarted;
	FOnReservationEnded OnReservationEnded;

protected:
	UPROPERTY()
	ANPCCharacter* ReservedBy = nullptr;

};
