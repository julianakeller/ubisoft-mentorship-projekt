#pragma once

#include "CoreMinimal.h" // for FGuid
#include "CustomerInstanceData.h"
#include "DesiredTagEntry.h"
#include "MentorshipProjekt/GameTime/GameTimeSubsystem.h"
#include "MentorshipProjekt/Purchasables/PurchasableInstance.h"
#include "MentorshipProjekt/Tags/MPGameplayTags.h"
#include "Household.generated.h"

struct FDesiredTagEntry;
class UPurchasableInstance;

USTRUCT()
struct FHouseholdItemStack
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UPurchasableDefinition> Definition;

	UPROPERTY()
	int32 Count = 0;

	// 0 = new, 1 = broken
	UPROPERTY()
	float AverageWear = 0.f;
	
	UPROPERTY()
	FInGameTime LastUpdateTime;
};

USTRUCT()
struct FHousehold
{
	GENERATED_BODY()
	
	FGuid HouseholdId = FGuid();
	
	TArray<FGuid> HouseholdMembers;
	
	TArray<FHouseholdItemStack> Inventory;
	
	// What items this customer wants to buy based on tags
	TMap<FGameplayTag, FDesiredTagEntry> ShoppingList;
	
	float SharedFunds = 0.f;
	
	void InitializeHousehold()
	{
		InitializeShoppingList();
	}
	
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
	
	void AddPurchasables(const TArray<UPurchasableInstance*>& Instances)
	{
		for (UPurchasableInstance* Instance : Instances)
		{
			if (!Instance || !Instance->Definition)
			{
				continue;
			}

			FHouseholdItemStack* ExistingStack = nullptr;

			for (FHouseholdItemStack& Stack : Inventory)
			{
				if (Stack.Definition == Instance->Definition)
				{
					ExistingStack = &Stack;
					break;
				}
			}

			const float IncomingWear = FMath::Clamp(0.5-Instance->Freshness, 0.f, 1.f);

			if (ExistingStack)
			{
				const int32 OldCount = ExistingStack->Count;
				const int32 NewCount = OldCount + 1;

				// average wear:
				ExistingStack->AverageWear = ((ExistingStack->AverageWear * OldCount) + IncomingWear) / NewCount;

				ExistingStack->Count = NewCount;
			}
			else
			{
				FHouseholdItemStack NewStack;
				NewStack.Definition = Instance->Definition;
				NewStack.Count = 1;
				NewStack.AverageWear = IncomingWear;

				Inventory.Add(NewStack);
			}
		}
	}
	
	void UpdateInventory()
	{
		for (int32 i = Inventory.Num() - 1; i >= 0; --i)
		{
			FHouseholdItemStack& Stack = Inventory[i];

			if (Stack.Count <= 0)
			{
				Inventory.RemoveAtSwap(i);
				continue;
			}

			// Increase wear over time
			Stack.AverageWear += 0.01f;

			Stack.AverageWear = FMath::Clamp(Stack.AverageWear, 0.f, 1.f);

			// Remove degraded stacks
			if (Stack.AverageWear >= 1.f)
			{
				Inventory.RemoveAtSwap(i);
			}
		}
	}
	
	void InitializeShoppingList()
	{
		ShoppingList.Empty();
		
		// Vegetables
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_Food_Type_Vegetable, 2, 3);

		// Flavours
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_Food_Flavour_Sweet, 1, 2);
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_Food_Flavour_Sour, 1, 1);
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_Food_Flavour_Salty, 1, 2);
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_Food_Flavour_Bitter, 1, 1);
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_Food_Flavour_Spicy, 2, 1);

		// Rarity
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_Rarity_Common, 3, 5);
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_Rarity_Uncommon, 2, 2);
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_Rarity_Rare, 1, 1);
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_Rarity_ExtremelyRare, 1, 1);

		// Luxury levels
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_LuxuryLevel_Cheap, 3, 4);
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_LuxuryLevel_Premium, 2, 2);
		AddShoppingListEntry(MPGameplayTags::TAG_Purchasable_LuxuryLevel_Luxury, 1, 1);
	}
	
	void AddShoppingListEntry(const FGameplayTag& Tag, int32 Priority, int32 Quantity)
	{
		if (!Tag.IsValid() || Quantity <= 0)
		{
			return;
		}

		if (FDesiredTagEntry* ExistingEntry = ShoppingList.Find(Tag))
		{
			ExistingEntry->QuantityRemaining += Quantity;

			// Keep highest priority
			ExistingEntry->Priority = FMath::Max(ExistingEntry->Priority, Priority);
			return;
		}

		FDesiredTagEntry NewEntry;
		NewEntry.Tag = Tag;
		NewEntry.Priority = Priority;
		NewEntry.QuantityRemaining = Quantity;

		ShoppingList.Add(Tag, NewEntry);
	}
};