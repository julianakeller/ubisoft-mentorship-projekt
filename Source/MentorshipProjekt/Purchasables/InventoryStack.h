#pragma once
#include "CoreMinimal.h"
#include "GameTimeSubsystem.h"
#include "PurchasableQuality.h"
#include "PurchasableFreshness.h"
#include "PurchasableDefinition.h"
#include "InventoryStack.generated.h"

USTRUCT()
struct FInventoryStackKey
{
	GENERATED_BODY()

	UPROPERTY()
	UPurchasableDefinition* Definition = nullptr;

	UPROPERTY()
	EPurchasableQuality Quality = EPurchasableQuality::Normal;

	UPROPERTY()
	EPurchasableFreshness Freshness = EPurchasableFreshness::New;

	bool operator==(const FInventoryStackKey& Other) const
	{
		return Definition == Other.Definition
			&& Quality == Other.Quality
			&& Freshness == Other.Freshness;
	}
};

USTRUCT(BlueprintType)
struct FInventoryStack
{
	GENERATED_BODY()

	UPROPERTY()
	UPurchasableDefinition* Definition = nullptr;

	UPROPERTY()
	FInventoryStackKey Key = FInventoryStackKey();
	
	UPROPERTY()
	int32 Quantity = 0;

	UPROPERTY()
	float AverageFreshness = 1.f;

	UPROPERTY()
	FInGameTime LastUpdateHour;
	
	bool IsValid() const
	{
		return Definition != nullptr && Quantity > 0;
	}
};

FORCEINLINE uint32 GetTypeHash(const FInventoryStackKey& Key)
{
	return HashCombine(
		HashCombine(GetTypeHash(Key.Definition), GetTypeHash((uint8)Key.Quality)),
		GetTypeHash((uint8)Key.Freshness)
	);
}