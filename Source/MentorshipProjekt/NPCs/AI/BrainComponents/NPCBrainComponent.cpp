// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBrainComponent.h"

// Sets default values for this component's properties
UNPCBrainComponent::UNPCBrainComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNPCBrainComponent::Initialize(ANPCCharacter* InOwner)
{
	OwnerCharacter = InOwner;

	if (!OwnerCharacter) return;
}

void UNPCBrainComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UNPCBrainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UNPCBrainComponent::SetReservedWorkstation(UReservationComponentBase* Workstation)
{
	ReservedWorkstation = Workstation;
}