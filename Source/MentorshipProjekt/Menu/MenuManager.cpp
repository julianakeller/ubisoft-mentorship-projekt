// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuManager.h"

#include "GameTimeSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "MentorshipProjekt/Public/GameTimeSubsystem.h"


void UMenuManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency(UGameTimeSubsystem::StaticClass());
	
	GameTimeSubsystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>();
	
	if (!GameTimeSubsystem)
	{
			UE_LOG(LogTemp, Error, TEXT("MenuManager: GameTimeSubsystem is nullptr"));
	}
}

void UMenuManager::ToggleMenu(TSubclassOf<UBaseMenuWidget> MenuClass, AInteractableBase* Source)
{
	if (!MenuClass)
	{
		// Close menu if menu class is nullptr
		if (CurrentMenu)
		{
			PauseTime(false);
			CloseCurrentMenu();
		}
		return;
	}

	if (IsMenuOpen())
	{
		PauseTime(false);
		CloseCurrentMenu();
	}
	else
	{
		PauseTime(true);
		OpenMenu(MenuClass, Source);
	}
}

void UMenuManager::PauseTime(bool Pause)
{
	GameTimeSubsystem->SetPaused(Pause);
}

void UMenuManager::OpenMenu(TSubclassOf<UBaseMenuWidget> MenuClass, AInteractableBase* Source)
{
	CurrentMenu = CreateWidget<UBaseMenuWidget>(GetWorld(), MenuClass);
	if (!CurrentMenu)
	{
		return;
	}
	
	CurrentMenu->InitializeMenu(Source);
	CurrentMenu->Open();
	LockPlayer();
	
	if (Source)
	{
		Source->SetInteractionState(EInteractionState::Interacting);
	}
}

void UMenuManager::CloseCurrentMenu()
{
	if (GameTimeSubsystem)
	{
		GameTimeSubsystem->CurrentTimeSpeed = ETimeSpeed::Normal;
	}
	
	if (AInteractableBase* Source = CurrentMenu->SourceInteractable)
	{
		Source->SetInteractionState(EInteractionState::Available);
	}
	
	CurrentMenu->Close();
	
	UnlockPlayer();
	
	CurrentMenu = nullptr;
}

void UMenuManager::LockPlayer()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		/*
		PC->SetIgnoreMoveInput(true);
		PC->SetIgnoreLookInput(true);
		*/
		PC->bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		if (CurrentMenu)
		{
			InputMode.SetWidgetToFocus(CurrentMenu->TakeWidget());
		}
		PC->SetInputMode(InputMode);
	}
}

void UMenuManager::UnlockPlayer()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
		PC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}
