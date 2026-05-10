// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameTimeSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameTimeWidget.generated.h"

class UTextBlock;

UCLASS()
class MENTORSHIPPROJEKT_API UGameTimeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UButton* PauseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* NormalSpeedButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FastSpeedButton;

	UPROPERTY(meta = (BindWidget))
	UButton* VeryFastSpeedButton;
	
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleTimeUpdated(const FInGameTime& NewTime);

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Time;
	
private:
	
	UFUNCTION()
	void OnPausedClicked();

	UFUNCTION()
	void OnNormalClicked();

	UFUNCTION()
	void OnFastClicked();

	UFUNCTION()
	void OnVeryFastClicked();
};
