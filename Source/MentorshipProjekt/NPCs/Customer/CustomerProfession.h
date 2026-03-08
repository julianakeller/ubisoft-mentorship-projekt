#pragma once
#include "ProfessionDataAsset.h"
#include "CustomerProfession.generated.h"

USTRUCT(BlueprintType)
struct FCustomerProfession
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<const UProfessionDataAsset> Definition;

	//Levels start at 0
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Level = 1;
	
	bool IsEmployed() const
	{
		return Definition != nullptr;
	}
	
	bool CanPromote() const
	{
		if (!Definition)
			return false;

		return Definition->GetLevelData(Level + 1) != nullptr;
	}

	float GetPromotionChance() const
	{
		if (!Definition)
			return 0.f;

		if (const FProfessionLevelData* LevelData = Definition->GetLevelData(Level))
		{
			return LevelData->PromotionChance;
		}

		return 0.f;
	}

	float GetDailyIncome() const
	{
		if (!Definition)
			return 0.f;

		if (const FProfessionLevelData* LevelData = Definition->GetLevelData(Level))
		{
			return Definition->BaseDailyIncome * LevelData->IncomeMultiplier;
		}

		return Definition->BaseDailyIncome;
	}
};
