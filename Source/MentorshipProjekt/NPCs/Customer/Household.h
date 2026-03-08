#pragma once

#include "CoreMinimal.h" // for FGuid
#include "Household.generated.h"

class UPurchasableInstance;
struct FCustomerInstanceData;

USTRUCT()
struct FHousehold
{
	GENERATED_BODY()
	
	FGuid HouseholdId;
	
	TMap<FGuid, FCustomerInstanceData*> HouseholdMembers;
	
	TArray<TObjectPtr<UPurchasableInstance>> HouseholdInventory;
	
	float SharedFunds = 0.f;
	
	bool CanAfford(const float Price) const
	{
		return SharedFunds >= Price;
	}

	void SpendFunds(const float Amount)
	{
		SharedFunds = FMath::Max(0.f, SharedFunds - Amount);
	}

	void AddFunds(const float Amount)
	{
		SharedFunds += Amount;
	}
};
