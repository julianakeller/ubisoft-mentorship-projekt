// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"
#include "MentorshipProjekt/Purchasables/PurchasableDefinition.h"
#include "Components/TextBlock.h"

void UInventoryItemWidget::InitializeItem(UPurchasableDefinition* Definition, int32 AmountValue)
{
	if (PurchasableName && Definition)
	{
		PurchasableName->SetText(FText::FromName(Definition->ItemName));
	}

	if (Amount)
	{
		Amount->SetText(FText::AsNumber(AmountValue));
	}
}