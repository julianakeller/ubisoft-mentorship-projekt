// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionRowWidget.h"

#include "InteractableWidgetTextComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "GameFramework/InputSettings.h"

void UInteractionRowWidget::SetupRow(const FInteractionEntry& Entry, bool bIsInteracting)
{
	CachedEntry = Entry;

	if (InteractionText)
	{
		InteractionText->SetText(
			bIsInteracting
			? Entry.InteractingText
			: Entry.AvailableText
		);
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
	{
		return;
	}

	UpdateButtonImage(
		PlayerController->GetPlatformUserId(),
		FInputDeviceId()
	);
}

void UInteractionRowWidget::UpdateButtonImage(FPlatformUserId UserId, FInputDeviceId InputDeviceId)
{
	if (!InputDeviceSubsystem)
	{
		return;
	}
	
	/*FHardwareDeviceIdentifier HardwareDeviceIdentifier = InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(UserId);
	
	FName LookupName = CachedEntry.InputKey.GetFName();
	
	if (UTexture2D** TextureFound = CachedEntry.DeviceNameToTexture.Find(HardwareDeviceIdentifier))
	{
		ButtonImage->SetBrushFromTexture(*TextureFound);
		return;
	}*/
	
	ButtonImage->SetBrushFromTexture(CachedEntry.ButtonTexture);
}

void UInteractionRowWidget::UpdateInteractionText(FText NewInteractionText)
{
	InteractionText->SetText(NewInteractionText);
}

void UInteractionRowWidget::NativeConstruct()
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
		&UInteractionRowWidget::UpdateButtonImage
	);

	InputDeviceSubsystem->OnInputHardwareDeviceChanged.AddDynamic(
		this,
		&UInteractionRowWidget::UpdateButtonImage
	);
}
