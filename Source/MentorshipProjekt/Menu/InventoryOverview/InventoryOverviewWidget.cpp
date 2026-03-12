// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryOverviewWidget.h"
#include "MentorshipProjekt/Purchasables/PurchasableManagerSubsystem.h"
#include "InventoryItemWidget.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UInventoryOverviewWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UWorld* World = GetWorld())
	{
		PurchasableManager = World->GetSubsystem<UPurchasableManagerSubsystem>();
		if (PurchasableManager)
		{
			PurchasableManager->OnInventoryChanged.AddDynamic(this, &UInventoryOverviewWidget::HandleInventoryChanged);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("InventoryOverviewWidget: PurchasableManager not found."));
		}
	}
	RebuildInventory();
}

void UInventoryOverviewWidget::NativeDestruct()
{
	PurchasableManager->OnInventoryChanged.RemoveAll(this);

	Super::NativeDestruct();
}

void UInventoryOverviewWidget::HandleInventoryChanged()
{
	RebuildInventory();
}

void UInventoryOverviewWidget::RebuildInventory()
{
	// ToDo improve performance, don't rebuild the entire inventory
	
	if (!InventoryContainer || !PurchasableManager)
	{
		return;
	}

	InventoryContainer->ClearChildren();

	UWorld* World = GetWorld();
	if (!World) return;

	TMap<UPurchasableDefinition*, int32> Purchasables = PurchasableManager->GetAllPurchasables();

	for (const TPair<UPurchasableDefinition*, int32>& Pair : Purchasables)
	{
		UPurchasableDefinition* Definition = Pair.Key;
		int32 Amount = Pair.Value;

		if (!Definition || !InventoryItemWidgetClass)
		{
			continue;
		}

		UInventoryItemWidget* ItemWidget = CreateWidget<UInventoryItemWidget>(World, InventoryItemWidgetClass);

		if (!ItemWidget) continue;

		ItemWidget->InitializeItem(Definition, Amount);

		InventoryContainer->AddChild(ItemWidget);
	}
}