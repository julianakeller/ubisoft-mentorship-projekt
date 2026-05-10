// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableWidgetTextComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InteractionRowWidget.generated.h"

/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UInteractionRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	UInputDeviceSubsystem* InputDeviceSubsystem = nullptr;
	
	UFUNCTION()
	void UpdateButtonImage(FPlatformUserId UserId, FInputDeviceId InputDeviceId);
	
public:
	void UpdateInteractionText(FText NewInteractionText);
	
	UFUNCTION(BlueprintCallable)
	void SetupRow(const FInteractionEntry& Entry, bool bIsInteracting);
	
protected:
	
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionText = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UImage* ButtonImage;
	
	UPROPERTY()
	FInteractionEntry CachedEntry;
	
};
