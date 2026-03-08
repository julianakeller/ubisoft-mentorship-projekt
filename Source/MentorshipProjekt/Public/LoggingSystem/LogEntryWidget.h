// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogMessage.h"
#include "Blueprint/UserWidget.h"
#include "LogEntryWidget.generated.h"

/**
 * 
 */

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class MENTORSHIPPROJEKT_API ULogEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void InitializeLogEntry(const FLogMessage& InMessage);

protected:

	virtual void NativeConstruct() override;

	void ApplyColor();
	void StartLifeTimer();
	void HandleFadeOutFinished();

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOutAnimation;
	
	UFUNCTION()
	void OnFadeAnimationFinished();

private:

	FLogMessage LogMessage;

	FTimerHandle LifeTimerHandle;
};
