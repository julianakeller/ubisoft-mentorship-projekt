// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../NPCCharacter.h"
#include "../AI/CustomerBrainComponents.h"
#include "GameFramework/Character.h"
#include "CustomerCharacter.generated.h"

UCLASS()
class MENTORSHIPPROJEKT_API ACustomerCharacter : public ANPCCharacter
{
	GENERATED_BODY()

public:
	ACustomerCharacter();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	UCustomerBrainComponents* BrainComponent; //Visit behavior

public:	
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
