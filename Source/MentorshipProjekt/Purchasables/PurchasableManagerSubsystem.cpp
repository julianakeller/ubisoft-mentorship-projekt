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
	if (Quality >= 1.4f) return EPurchasableQuality::Excellent;
	if (Quality >= 1.1f) return EPurchasableQuality::Good;
	if (Quality >= 0.9f) return EPurchasableQuality::Normal;
	if (Quality >= 0.7f) return EPurchasableQuality::Poor;

	return EPurchasableQuality::Terrible;
}

UPurchasableInstance* UPurchasableManagerSubsystem::ProducePurchasable(UPurchasableDefinition* Definition, int32 WorkerSkillLevel)
{
	if (!Definition)
	{
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	UPurchasableInstance* Instance = NewObject<UPurchasableInstance>(this);
	if (!Instance)
	{
		return nullptr;
	}

	Instance->Initialize(Definition, 1.f, WorkerSkillLevel);

	EPurchasableQuality QualityLevel = GetQualityLevel(Instance->Quality);
	EPurchasableFreshness FreshnessLevel = GetFreshnessLevel(Instance->Freshness);

	// ToDo ingredients freshness and quality influence outcome freshness and quality
	
	FInventoryStackKey Key;
	Key.Definition = Definition;
	Key.Quality = QualityLevel;
	Key.Freshness = FreshnessLevel;

	FInventoryStack& Stack = Inventory.FindOrAdd(Key);
	
	Stack.Key = Key;
	Stack.Quantity = 0;
	Stack.AverageFreshness = Instance->Freshness;
	Stack.LastUpdateHour = Instance->CreationTime;

	float TotalFreshness = Stack.AverageFreshness * Stack.Quantity;
	TotalFreshness += Instance->Freshness;

	Stack.Quantity++;
	Stack.AverageFreshness = TotalFreshness / Stack.Quantity;

	OnInventoryChanged.Broadcast();

	return Instance;
}

bool UPurchasableManagerSubsystem::RemovePurchasableIfAvailable(UPurchasableDefinition* Definition, int32 Count)
{
	if (!Definition || Count <= 0)
	{
		return false;
	}

	// Collect all stacks of this definition
	TArray<FInventoryStack*> MatchingStacks;
	int32 TotalAvailable = 0;

	for (TPair<FInventoryStackKey, FInventoryStack>& Pair : Inventory)
	{
		FInventoryStack& Stack = Pair.Value;

		if (Pair.Key.Definition == Definition && Stack.Quantity > 0)
		{
			MatchingStacks.Add(&Stack);
			TotalAvailable += Stack.Quantity;
		}
	}

	// Do nothing if insufficient quantity
	if (TotalAvailable < Count)
	{
		return false;
	}

	// Use lowest freshness items first, then lowest quality items
	MatchingStacks.Sort([](const FInventoryStack& A, const FInventoryStack& B)
	{
		if (A.Key.Freshness != B.Key.Freshness)
		{
			return (uint8)A.Key.Freshness > (uint8)B.Key.Freshness;
		}

		return (uint8)A.Key.Quality > (uint8)B.Key.Quality;
	});

	int32 Remaining = Count;

	for (FInventoryStack* Stack : MatchingStacks)
	{
		if (Remaining <= 0)
		{
			break;
		}

		int32 RemoveAmount = FMath::Min(Stack->Quantity, Remaining);

		Stack->Quantity -= RemoveAmount;
		Remaining -= RemoveAmount;
	}

	// Remove empty stacks
	for (auto It = Inventory.CreateIterator(); It; ++It)
	{
		if (It.Value().Quantity <= 0)
		{
			It.RemoveCurrent();
		}
	}

	OnInventoryChanged.Broadcast();

	return true;
}

TArray<FInventoryStack> UPurchasableManagerSubsystem::GetAllStacks() const
{
	TArray<FInventoryStack> Result;

	for (const TPair<FInventoryStackKey, FInventoryStack>& Pair : Inventory)
	{
		if (Pair.Value.IsValid())
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

TMap<UPurchasableDefinition*, int32> UPurchasableManagerSubsystem::GetAllPurchasables() const
{
	TMap<UPurchasableDefinition*, int32> Result;

	for (const TPair<FInventoryStackKey, FInventoryStack>& Pair : Inventory)
	{
		const FInventoryStack& Stack = Pair.Value;

		if (!Pair.Key.Definition)
		{
			continue;
		}

		int32& Count = Result.FindOrAdd(Pair.Key.Definition);
		Count += Stack.Quantity;
	}
	return Result;
}

float UPurchasableManagerSubsystem::GetTotalInventoryValue() const
{
	return 0.f; //ToDo
}

int32 UPurchasableManagerSubsystem::GetPurchasableCountByDefinition(UPurchasableDefinition* Definition) const
{
	if (!Definition)
	{
		return 0;
	}

	int32 Count = 0;

	for (const TPair<FInventoryStackKey, FInventoryStack>& Pair : Inventory)
	{
		if (Pair.Key.Definition == Definition)
		{
			Count += Pair.Value.Quantity;
		}
	}

	return Count;
}

void UPurchasableManagerSubsystem::HandleHourChanged(const FInGameTime& NewTime)
{
	for (auto& Pair : Inventory)
	{
		UpdateStackFreshness(Pair.Value, NewTime);
	}
}

void UPurchasableManagerSubsystem::UpdateStackFreshness(FInventoryStack& Stack, FInGameTime CurrentTime) const
{
	// ToDo Remove stack if expired
}

void UPurchasableManagerSubsystem::RemoveExpiredItems(FInventoryStack& Stack)
{
	OnInventoryChanged.Broadcast();
}
