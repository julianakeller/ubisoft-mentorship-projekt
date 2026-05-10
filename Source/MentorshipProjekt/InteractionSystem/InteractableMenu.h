// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "MentorshipProjekt/Menu/BaseMenuWidget.h"
#include "InteractableMenu.generated.h"

/**
 * 
 */

//class is no longer needed!!! Will be deleted later
UCLASS()
class MENTORSHIPPROJEKT_API AInteractableMenu : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	virtual void HandleInteraction(AActor* Character) override;
	virtual bool CanBeInteracted() const override;
	
private:
	bool IsMenuOpen() const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu")
	TSubclassOf<UBaseMenuWidget> ShiftMenuWidgetClass;
};
