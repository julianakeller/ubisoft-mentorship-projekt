// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AreaSign.generated.h"

UCLASS()
class MENTORSHIPPROJEKT_API AAreaSign : public AActor
{
	GENERATED_BODY()
	
public:	
	AAreaSign();

protected:
	virtual void BeginPlay() override;

};
