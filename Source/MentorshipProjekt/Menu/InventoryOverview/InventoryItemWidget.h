// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

class UTextBlock;
class UPurchasableDefinition;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION()
	void InitializeItem(UPurchasableDefinition* Definition, int32 AmountValue);
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PurchasableName;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Amount;
};
