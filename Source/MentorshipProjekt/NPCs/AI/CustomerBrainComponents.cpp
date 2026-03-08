// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerBrainComponents.h"

#include "MentorshipProjekt/NPCs/Customer/CustomerCharacter.h"

// Sets default values for this component's properties
UCustomerBrainComponents::UCustomerBrainComponents()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 1.0f; // 1 second
}

void UCustomerBrainComponents::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACustomerCharacter>(GetOwner());
	
}

void UCustomerBrainComponents::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

