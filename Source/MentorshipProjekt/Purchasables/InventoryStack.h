#pragma once
#include "CoreMinimal.h"
#include "MentorshipProjekt/GameTime/GameTimeSubsystem.h"
#include "PurchasableQuality.h"
#include "PurchasableFreshness.h"
#include "PurchasableDefinition.h"
#include "InventoryStack.generated.h"

class UPurchasableInstance;

USTRUCT(BlueprintType)
struct FInventoryStackKey
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	UPurchasableDefinition* Definition = nullptr;

	UPROPERTY(BlueprintReadOnly)
	EPurchasableQuality Quality = EPurchasableQuality::Normal;

	UPROPERTY(BlueprintReadOnly)
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
	FInventoryStackKey Key = FInventoryStackKey();
	
	UPROPERTY(BlueprintReadOnly)
	int32 Quantity = 0;

	UPROPERTY()
	float AverageFreshness = 1.f;

	UPROPERTY()
	FInGameTime LastUpdateHour;
	
	bool IsValid() const
	{
		return Quantity > 0;
	}
};

FORCEINLINE uint32 GetTypeHash(const FInventoryStackKey& Key)
{
	return HashCombine(
		HashCombine(GetTypeHash(Key.Definition), GetTypeHash((uint8)Key.Quality)),
		GetTypeHash((uint8)Key.Freshness)
	);
}

USTRUCT(BlueprintType)
struct FPurchaseResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<UPurchasableInstance*> Instances;

	UPROPERTY(BlueprintReadOnly)
	int32 AmountRemoved = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 AmountRequested = 0;
};