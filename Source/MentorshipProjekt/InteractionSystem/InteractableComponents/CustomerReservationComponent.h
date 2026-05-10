// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "ReservationComponentBase.h"
#include "Components/ArrowComponent.h"
#include "CustomerReservationComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UCustomerReservationComponent : public UReservationComponentBase
{
	GENERATED_BODY()
	
public:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TObjectPtr<UArrowComponent> CustomerInteractionPoint;
};
