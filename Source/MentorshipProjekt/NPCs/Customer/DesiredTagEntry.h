#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DesiredTagEntry.generated.h"

USTRUCT(BlueprintType)
struct FDesiredTagEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuantityRemaining = 1;
};