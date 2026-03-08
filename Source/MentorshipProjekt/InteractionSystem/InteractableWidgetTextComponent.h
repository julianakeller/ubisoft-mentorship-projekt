// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "InteractableWidgetTextComponent.generated.h"

/**
 * 
 */
class UMenuManager;

UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UInteractableWidgetTextComponent : public UInteractableComponentBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:

	// Default text
	UPROPERTY(EditAnywhere, Category="Text")
	FText AvailableText = FText::FromString("Open Menu");

	// Text when interacting
	UPROPERTY(EditAnywhere, Category="Text")
	FText InteractingText = FText::FromString("Close Menu");
	
	virtual void OnRangeEntered(AActor* Interactor) override;
	virtual void OnRangeExited(AActor* Interactor) override;
	virtual void OnInteract(AActor* Interactor) override;
	
private:
	void UpdateText() const;
};
