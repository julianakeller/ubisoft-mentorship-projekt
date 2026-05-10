// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"

class UNPCBrainComponent;

UCLASS()
class MENTORSHIPPROJEKT_API ANPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCCharacter();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UNPCBrainComponent* BrainComponent = nullptr;

};
