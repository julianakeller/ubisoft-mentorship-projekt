// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableWorkstationComponent.h"

UInteractableWorkstationComponent::UInteractableWorkstationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableWorkstationComponent::BeginPlay()
{
	Super::BeginPlay();

	// Ensure AvailablePurchasables == ProductionSettings indices
	if (ProductionSettings.Num() == 0)
	{
		for (TObjectPtr<UPurchasableDefinition> Purchasable : AvailablePurchasables)
		{
			FWorkstationProductionSetting Setting;
			Setting.Purchasable = Purchasable;
			Setting.bEnabled = false;
			Setting.TargetAmount = 0;
			
			ProductionSettings.Add(Setting);
		}
	}
}

FWorkstationProductionSetting* UInteractableWorkstationComponent::GetHighestPriorityEnabledSetting()
{
	FWorkstationProductionSetting* BestSetting = nullptr;
	int32 BestPriority = TNumericLimits<int32>::Lowest();

	for (FWorkstationProductionSetting& Setting : ProductionSettings)
	{
		if (!Setting.bEnabled)
		{
			continue;
		}

		if (Setting.TargetAmount <= 0)
		{
			continue;
		}

		if (Setting.Priority > BestPriority)
		{
			BestPriority = Setting.Priority;
			BestSetting = &Setting;
		}
	}

	return BestSetting;
}

FWorkstationProductionSetting* UInteractableWorkstationComponent::GetSetting(int32 Index)
{
	if (ProductionSettings.IsValidIndex(Index))
	{
		return &ProductionSettings[Index];
	}

	return nullptr;
}

void UInteractableWorkstationComponent::SortProductionSettings()
{
	ProductionSettings.Sort([](const FWorkstationProductionSetting& A, const FWorkstationProductionSetting& B)
	{
		// Higher priority first. If equal, keep original order
		return A.Priority > B.Priority;
	});
}
