// Fill out your copyright notice in the Description page of Project Settings.


#include "EconomyOverviewWidget.h"

#include "FamilyEconomySubsystem.h"
#include "Components/TextBlock.h"

void UEconomyOverviewWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UGameInstance* GI = GetGameInstance())
	{
		EconomySubsystem = GI->GetSubsystem<UFamilyEconomySubsystem>();

		if (EconomySubsystem)
		{
			EconomySubsystem->OnGoldChanged.AddDynamic(this, &UEconomyOverviewWidget::RefreshGold);

			RefreshGold();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UEconomyOverviewWidget: Could not find EconomySubsystem"));
		}
	}
}

void UEconomyOverviewWidget::NativeDestruct()
{
	if (EconomySubsystem)
	{
		EconomySubsystem->OnGoldChanged.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UEconomyOverviewWidget::RefreshGold()
{
	if (!EconomySubsystem || !ReputationText)
	{
		return;
	}

	const int32 RoundedGold = FMath::RoundToInt(EconomySubsystem->GetGold());

	GoldText->SetText(FText::AsNumber(RoundedGold));
}