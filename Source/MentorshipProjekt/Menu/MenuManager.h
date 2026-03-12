// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BaseMenuWidget.h"
#include "MenuManager.generated.h"

class UGameTimeSubsystem;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UMenuManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void ToggleMenu(TSubclassOf<UBaseMenuWidget> MenuClass, AInteractableBase* Source = nullptr);

	void CloseCurrentMenu();

	bool IsMenuOpen() const { return CurrentMenu != nullptr; }

private:
	UPROPERTY()
	UBaseMenuWidget* CurrentMenu = nullptr;

	void LockPlayer();

	void UnlockPlayer();
	
	void OpenMenu(TSubclassOf<UBaseMenuWidget> MenuClass, AInteractableBase* Source = nullptr);
	
	void PauseTime(bool Pause);
	
	UGameTimeSubsystem* GameTimeSubsystem = nullptr;
};
