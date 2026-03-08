// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCBrainComponent.generated.h"


UCLASS( ClassGroup=(AI), meta=(BlueprintSpawnableComponent) )
class MENTORSHIPPROJEKT_API UNPCBrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNPCBrainComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
