#include "MPGameplayTags.h"

namespace MPGameplayTags
{
	// Purchasable Root
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable, "Purchasable");
    
	// Food Categories
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food, "Purchasable.Food");
    
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Vegetable, "Purchasable.Food.Type.Vegetable");
	
	// Flavor profiles
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Sweet, "Purchasable.Food.Flavour.Sweet");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Sour, "Purchasable.Food.Flavour.Sour");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Salty, "Purchasable.Food.Flavour.Salty");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Bitter, "Purchasable.Food.Flavour.Bitter");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Spicy, "Purchasable.Food.Flavour.Spicy");

	// Rarity
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Rarity_Common, "Purchasable.Rarity.Common");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Rarity_Uncommon, "Purchasable.Rarity.Uncommon");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Rarity_Rare, "Purchasable.Rarity.Rare");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Rarity_ExtremelyRare, "Purchasable.Rarity.ExtremelyRare");

	// Luxury / Necessity Levels
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_LuxuryLevel_Luxury, "Purchasable.LuxuryLevel.Luxury");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_LuxuryLevel_Daily, "Purchasable.LuxuryLevel.Daily");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_LuxuryLevel_Premium, "Purchasable.LuxuryLevel.Premium");
}