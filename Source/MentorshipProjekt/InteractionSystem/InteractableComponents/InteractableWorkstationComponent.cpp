// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableWorkstationComponent.h"

#include "MentorshipProjekt/Purchasables/PurchasableManagerSubsystem.h"
#include "MentorshipProjekt/Purchasables/RecipeDefinition.h"

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
			Setting.bEnabled = true;
			Setting.TargetAmount = 100;
			
			ProductionSettings.Add(Setting);
		}
	}
}

const FWorkstationProductionSetting* UInteractableWorkstationComponent::GetHighestPriorityEnabledSetting() const
{
	const FWorkstationProductionSetting* BestSetting = nullptr;
	int32 BestPriority = TNumericLimits<int32>::Lowest();

	for (const FWorkstationProductionSetting& Setting : ProductionSettings)
	{
		if (!Setting.bEnabled)
		{
			continue;
		}
		
		if (!Setting.Purchasable)
		{
			continue;
		}

		if (Setting.Purchasable->Recipe)
		{
			if (!PurchasableManager || !PurchasableManager->IngredientsAvailable(Setting.Purchasable->Recipe->Ingredients))
			{
				continue;
			}
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

UPurchasableDefinition* UInteractableWorkstationComponent::GetHighestPriorityPurchasable()
{
	return GetHighestPriorityEnabledSetting()->Purchasable;
}

const FWorkstationProductionSetting* UInteractableWorkstationComponent::GetProductionSetting(int32 Index) const
{
	if (!ProductionSettings.IsValidIndex(Index))
	{
		return nullptr;
	}

	return &ProductionSettings[Index];
}

void UInteractableWorkstationComponent::SetProductionSetting(const FWorkstationProductionSetting Setting)
{
	if (!Setting.Purchasable)
	{
		return;
	}

	for (FWorkstationProductionSetting& Existing : ProductionSettings)
	{
		if (Existing.Purchasable == Setting.Purchasable)
		{
			Existing = Setting;

			NotifyProductionSettingsChanged();
			return;
		}
	}

}

void UInteractableWorkstationComponent::NotifyProductionSettingsChanged()
{
	OnProductionSettingsChanged.Broadcast(this);
}

void UInteractableWorkstationComponent::RebuildSortedIndices()
{
	SortedSettingIndices.Reset();

	for (int32 i = 0; i < ProductionSettings.Num(); ++i)
	{
		SortedSettingIndices.Add(i);
	}

	SortedSettingIndices.Sort([this](int32 A, int32 B)
	{
		const FWorkstationProductionSetting& SA = ProductionSettings[A];
		const FWorkstationProductionSetting& SB = ProductionSettings[B];

		return SA.Priority > SB.Priority;
	});
}

const FWorkstationProductionSetting* UInteractableWorkstationComponent::GetSettingSorted(int32 SortedIndex) const
{
	if (!SortedSettingIndices.IsValidIndex(SortedIndex))
	{
		return nullptr;
	}

	int32 RealIndex = SortedSettingIndices[SortedIndex];
	return ProductionSettings.IsValidIndex(RealIndex) ? &ProductionSettings[RealIndex] : nullptr;
}