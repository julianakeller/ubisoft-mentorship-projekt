// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include  "GameFramework/InputDeviceSubsystem.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;

UCLASS()
class MENTORSHIPPROJEKT_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionText = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UImage* ButtonImage;
	
	UPROPERTY()
	UInputDeviceSubsystem* InputDeviceSubsystem = nullptr;
	
	UFUNCTION()
	void UpdateButtonImage(FPlatformUserId UserId, FInputDeviceId InputDeviceId);
	
	UPROPERTY(EditDefaultsOnly, Category="Parameters")
	TMap<FName, UTexture2D*> DeviceNameToTexture;
	
public:
	void UpdateInteractionText(FText NewInteractionText);
	
protected:
	virtual void NativeConstruct() override;
};
