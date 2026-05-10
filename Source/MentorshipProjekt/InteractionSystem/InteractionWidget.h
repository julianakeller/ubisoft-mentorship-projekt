// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableWidgetTextComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include  "GameFramework/InputDeviceSubsystem.h"
#include "InteractionWidget.generated.h"

class UInteractionRowWidget;
/**
 * 
 */
class UImage;
class UTextBlock;

UCLASS()
class MENTORSHIPPROJEKT_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	void PopulateRows(TArray<FInteractionEntry> Rows, bool bIsInteracting);
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* InteractionWidgetRowContainer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UInteractionRowWidget> InteractionRowWidgetClass;
};
