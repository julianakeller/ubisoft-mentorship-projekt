#pragma once

#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

namespace MPGameplayTags
{
	//Health Condition Triggers
	
	//Health Condition Relief Factors
	
	//Purchasable Root
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable);
	
	//Food Categories
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Vegetable);
	
	//Flavor profiles
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Sweet);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Sour);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Salty);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Bitter);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Spicy);

	//Rarity
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Rarity_Common);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Rarity_Uncommon);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Rarity_Rare);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Rarity_ExtremelyRare);
	
	//Luxury Levels
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_LuxuryLevel_Luxury);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_LuxuryLevel_Daily);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_LuxuryLevel_Premium);
}
