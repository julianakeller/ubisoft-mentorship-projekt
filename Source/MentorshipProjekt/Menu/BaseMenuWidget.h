// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"
#include "BaseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UBaseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Open();
	virtual void Close();
	virtual void InitializeMenu(AInteractableBase* Source);
	
	UPROPERTY(BlueprintReadOnly)
	AInteractableBase* SourceInteractable = nullptr;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnMenuOpened();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnMenuClosed();
};
