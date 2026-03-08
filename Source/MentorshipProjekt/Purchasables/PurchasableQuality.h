#pragma once

#include "CoreMinimal.h"
#include "PurchasableQuality.generated.h"

UENUM(BlueprintType)
enum class EPurchasableQuality : uint8
{
	Poor        UMETA(DisplayName="Poor"),
	Normal      UMETA(DisplayName="Normal"),
	Good        UMETA(DisplayName="Good"),
	Excellent   UMETA(DisplayName="Excellent")
};
