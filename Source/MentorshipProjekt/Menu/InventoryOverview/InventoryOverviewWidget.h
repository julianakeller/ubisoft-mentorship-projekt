// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryOverviewWidget.generated.h"

class UPurchasableManagerSubsystem;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UInventoryOverviewWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;

protected:

	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* InventoryContainer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInventoryItemWidget> InventoryItemWidgetClass;

private:

	UFUNCTION()
	void HandleInventoryChanged();

	void RebuildInventory();
	
	UPurchasableManagerSubsystem* PurchasableManager = nullptr;
};
