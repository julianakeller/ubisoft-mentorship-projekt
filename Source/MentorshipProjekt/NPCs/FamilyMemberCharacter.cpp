// Fill out your copyright notice in the Description page of Project Settings.


#include "FamilyMemberCharacter.h"
#include "AI/FamilyMemberBrainComponent.h"

AFamilyMemberCharacter::AFamilyMemberCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	BrainComponent = CreateDefaultSubobject<UFamilyMemberBrainComponent>(TEXT("FamilyBrainComponent"));
}

void AFamilyMemberCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFamilyMemberCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFamilyMemberCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

