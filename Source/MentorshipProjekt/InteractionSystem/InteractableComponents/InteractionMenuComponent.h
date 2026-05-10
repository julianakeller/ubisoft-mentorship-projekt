// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "InteractionMenuComponent.generated.h"

class UBaseMenuWidget;
/**
 * 
 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UInteractionMenuComponent : public UInteractableComponentBase
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBaseMenuWidget> MenuClass;

	virtual void OnInteract(AActor* Interactor) override;
};
