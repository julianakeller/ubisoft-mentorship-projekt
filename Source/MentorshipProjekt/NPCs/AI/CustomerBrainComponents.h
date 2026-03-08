// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBrainComponent.h"
#include "Components/ActorComponent.h"
#include "CustomerBrainComponents.generated.h"


class ACustomerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MENTORSHIPPROJEKT_API UCustomerBrainComponents : public UNPCBrainComponent
{
	GENERATED_BODY()

public:	
	UCustomerBrainComponents();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	ACustomerCharacter* OwnerCharacter = nullptr;
		
};
