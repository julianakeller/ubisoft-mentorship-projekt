// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCBrainComponent.generated.h"


class ANPCCharacter;
class UReservationComponentBase;
class AInteractableBase;
class UWSInteractionComponentBase;

UCLASS( ClassGroup=(AI), meta=(BlueprintSpawnableComponent) )
class MENTORSHIPPROJEKT_API UNPCBrainComponent : public UActorComponent
{
	GENERATED_BODY()
	
	
public:	
	UNPCBrainComponent();
	
	void Initialize(ANPCCharacter* InOwner);
	
	UPROPERTY()
	ANPCCharacter* OwnerCharacter = nullptr;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Called by BTTask when customer reaches workstation
	void SetReservedWorkstation(UReservationComponentBase* Workstation);
	
	UPROPERTY()
	UReservationComponentBase* ReservedWorkstation = nullptr;
	
	virtual AInteractableBase* GetTargetWorkstation() {return nullptr;};
};
