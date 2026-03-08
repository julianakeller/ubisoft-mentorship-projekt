// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInteractionWidget::UpdateButtonImage(FPlatformUserId UserId, FInputDeviceId InputDeviceId)
{
	if (!InputDeviceSubsystem)
	{
		return;
	}
	
	FHardwareDeviceIdentifier HardwareDeviceIdentifier = InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(UserId);
	
	if (UTexture2D** TextureFound = DeviceNameToTexture.Find(HardwareDeviceIdentifier.HardwareDeviceIdentifier))
	{
		ButtonImage->SetBrushFromTexture(*TextureFound);
		return;
	}
}

void UInteractionWidget::UpdateInteractionText(FText NewInteractionText)
{
	InteractionText->SetText(NewInteractionText);
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	InputDeviceSubsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();
	
	if (!InputDeviceSubsystem)
	{
		return;
	}
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	
	if (!PlayerController)
	{
		return;
	}
	
	FPlatformUserId UserId = PlayerController->GetPlatformUserId();
	
	FInputDeviceId InputDeviceId;
	
	UpdateButtonImage(UserId, InputDeviceId);
	
	// Prevent duplicate binding by removing first
	InputDeviceSubsystem->OnInputHardwareDeviceChanged.RemoveDynamic(
		this,
		&UInteractionWidget::UpdateButtonImage
	);

	InputDeviceSubsystem->OnInputHardwareDeviceChanged.AddDynamic(
		this,
		&UInteractionWidget::UpdateButtonImage
	);
}
