// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerCharacter.h"

ACustomerCharacter::ACustomerCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	BrainComponent = CreateDefaultSubobject<UCustomerBrainComponents>(TEXT("CustomerComponent"));
}

void ACustomerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACustomerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACustomerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

