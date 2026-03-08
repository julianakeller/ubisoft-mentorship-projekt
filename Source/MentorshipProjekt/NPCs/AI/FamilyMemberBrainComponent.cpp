// Fill out your copyright notice in the Description page of Project Settings.


#include "FamilyMemberBrainComponent.h"

UFamilyMemberBrainComponent::UFamilyMemberBrainComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 1.0f; // 1 second

}

void UFamilyMemberBrainComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AFamilyMemberCharacter>(GetOwner());
}

void UFamilyMemberBrainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

