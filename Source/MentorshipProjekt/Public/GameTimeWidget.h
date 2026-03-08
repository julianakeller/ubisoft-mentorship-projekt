// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameTimeSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "GameTimeWidget.generated.h"

class UTextBlock;

UCLASS()
class MENTORSHIPPROJEKT_API UGameTimeWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleTimeUpdated(const FInGameTime& NewTime);

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Time;
};
