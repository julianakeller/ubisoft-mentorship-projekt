// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EconomyOverviewWidget.generated.h"

class UFamilyEconomySubsystem;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UEconomyOverviewWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ReputationText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldText;

	UFUNCTION()
	void RefreshGold();

private:

	UPROPERTY()
	TObjectPtr<UFamilyEconomySubsystem> EconomySubsystem;
};
