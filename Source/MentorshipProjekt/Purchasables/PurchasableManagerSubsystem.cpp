// Fill out your copyright notice in the Description page of Project Settings.


#include "PurchasableManagerSubsystem.h"
#include "PurchasableDefinition.h"
#include "PurchasableInstance.h"
#include "GameTimeSubsystem.h"
#include "PurchasableFreshness.h"
#include "Engine/GameInstance.h"

void UPurchasableManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (UWorld* World = GetWorld())
	{
		TimeSubsystem = World->GetSubsystem<UGameTimeSubsystem>();
		if (TimeSubsystem)
		{
			TimeSubsystem->OnHourChanged.AddDynamic(this, &UPurchasableManagerSubsystem::HandleHourChanged);
		}
	}
}

void UPurchasableManagerSubsystem::Deinitialize()
{
	if (TimeSubsystem)
	{
		TimeSubsystem->OnHourChanged.RemoveAll(this);
	}
	Inventory.Empty();

	Super::Deinitialize();
}

EPurchasableFreshness UPurchasableManagerSubsystem::GetFreshnessLevel(const float Freshness)
{
	if (Freshness >= 0.8f) return EPurchasableFreshness::New;
	if (Freshness >= 0.6f) return EPurchasableFreshness::Fresh;
	if (Freshness >= 0.4f) return EPurchasableFreshness::Aging;
	if (Freshness >= 0.2f) return EPurchasableFreshness::Stale;
	return EPurchasableFreshness::Spoiled;
}

EPurchasableQuality UPurchasableManagerSubsystem::GetQualityLevel(const float Quality)
{
	return EPurchasableQuality::Excellent; //ToDo
}

UPurchasableInstance* UPurchasableManagerSubsystem::ProducePurchasable(UPurchasableDefinition* Definition, int32 WorkerSkillLevel)
{
	if (!Definition)
	{
		return nullptr;
	}

	//ToDo
	return nullptr;
}

bool UPurchasableManagerSubsystem::BuyPurchasable(UPurchasableDefinition* Definition, int32 Count)
{
	return false;  //ToDo
}

TArray<FInventoryStack> UPurchasableManagerSubsystem::GetAllStacks() const
{
	return TArray<FInventoryStack>(); //ToDo
}

float UPurchasableManagerSubsystem::GetTotalInventoryValue() const
{
	return 0.f; //ToDo
}

int32 UPurchasableManagerSubsystem::GetPurchasableCountByDefinition(UPurchasableDefinition* Definition) const
{
	return 0; //ToDo
}

void UPurchasableManagerSubsystem::HandleHourChanged(const FInGameTime& NewTime)
{
}

void UPurchasableManagerSubsystem::UpdateStackFreshness(FInventoryStack& Stack, FInGameTime CurrentTime) const
{
}

void UPurchasableManagerSubsystem::RemoveExpiredItems()
{
}
