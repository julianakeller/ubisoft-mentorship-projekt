// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBrainComponent.h"

// Sets default values for this component's properties
UNPCBrainComponent::UNPCBrainComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 1.0f; // 1 second
}

void UNPCBrainComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UNPCBrainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

