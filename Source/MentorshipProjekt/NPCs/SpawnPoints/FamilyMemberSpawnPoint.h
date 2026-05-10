// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FamilyMemberSpawnPoint.generated.h"

UCLASS()
class MENTORSHIPPROJEKT_API AFamilyMemberSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFamilyMemberSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
