// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "ReservationComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UReservationComponent : public UInteractableComponentBase
{
	GENERATED_BODY()
	
public:
	bool TryReserve(AActor* Actor);
	void ClearReservation();
	bool IsReserved() const;

protected:
	UPROPERTY()
	AActor* ReservedBy = nullptr;

	virtual void OnInteract(AActor* Interactor) override;
};
