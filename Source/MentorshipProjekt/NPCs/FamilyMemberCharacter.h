// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacter.h"
#include "GameFramework/Character.h"
#include "FamilyMemberCharacter.generated.h"

class UFamilyMemberBrainComponent;

UCLASS()
class MENTORSHIPPROJEKT_API AFamilyMemberCharacter : public ANPCCharacter
{
	GENERATED_BODY()

public:
	AFamilyMemberCharacter();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	UFamilyMemberBrainComponent* BrainComponent; //Shift and work simulation

public:	
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
