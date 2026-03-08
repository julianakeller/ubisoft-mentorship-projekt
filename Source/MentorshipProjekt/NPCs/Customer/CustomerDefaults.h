#pragma once

#include "CoreMinimal.h"
#include "CustomerDefaults.generated.h"

USTRUCT(BlueprintType)
struct FCustomerDefaults
{
	GENERATED_BODY()

	static constexpr float MinAge = 18.f;
	static constexpr float MaxAge = 70.f;
	static constexpr float MinFunds = 50.f;
	static constexpr float MaxFunds = 500.f;
	static constexpr float DefaultGoodwill = 50.f;

	static const TArray<FString> MaleFirstNames;

	static const TArray<FString> FemaleFirstNames;

	static const TArray<FString> LastNames;
};
