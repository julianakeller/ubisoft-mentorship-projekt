// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogMessage.h"
#include "Blueprint/UserWidget.h"
#include "LogWindowWidget.generated.h"

/**
 * 
 */

class UVerticalBox;
class ULogEntryWidget;

UCLASS()
class MENTORSHIPPROJEKT_API ULogWindowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void AddLogMessage(const FLogMessage& Message);

protected:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* LogContainer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULogEntryWidget> LogEntryClass;

};
