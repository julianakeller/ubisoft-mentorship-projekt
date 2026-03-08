#pragma once

#include "CoreMinimal.h"
#include "PurchasableFreshness.generated.h"

UENUM(BlueprintType)
enum class EPurchasableFreshness : uint8
{
	New,
	Fresh,
	Aging,
	Stale,
	Spoiled
};