#pragma once

#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

namespace MPGameplayTags
{
#pragma region Health Conditions
	
	//Health Condition Triggers
	
	//Health Condition Relief Factors
	
#pragma endregion
	
#pragma region Actions
	
	// Actions Root
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action);

	// Food-related actions
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Cooking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_PreparingFood);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Bartending);

	// Farming actions
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Sowing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Harvesting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Watering);

	// Production / workstation actions
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_FillingBarrel);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_TendingCounter);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_ServingCustomer);
	
#pragma endregion
	
#pragma region Skills
	
	// Skills Root
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill);

	// Individual Skills
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Gardening);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Cooking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Fermenting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Bartending);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Medicine);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Chemistry);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Teaching);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Crafting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Bartering);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_AnimalHandling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Baking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Painting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Sculpting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Sewing);
	
#pragma endregion
	
#pragma region Purchasables
	
	//Purchasable Root
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable);
	
#pragma region Consumable

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Consumable);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Consumable_Edible);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Consumable_Drinkable);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Consumable_Healing);

#pragma endregion
	
#pragma region Categories
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Drink);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Medicine);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Cosmetic);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Clothing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Tool);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Furniture);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Material);
	
#pragma endregion
	
#pragma region Medicine

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Medicine_Effect);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Medicine_Effect_Healing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Medicine_Effect_Energy);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Medicine_Effect_Calm);

#pragma endregion

#pragma region Cosmetic

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Cosmetic_Type);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Cosmetic_Type_Perfume);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Cosmetic_Type_Soap);

#pragma endregion
	
#pragma region Food
	
#pragma region Food Type
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Vegetable);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Fruit);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Meat);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Fish);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Grain);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Dairy);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Herb);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Spice);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Bread);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Dessert);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Soup);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Stew);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Type_Preserved);
	
#pragma endregion
	
#pragma region Food Nutrition

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Nutrition);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Nutrition_Healthy);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Nutrition_Unhealthy);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Nutrition_HighProtein);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Nutrition_HighEnergy);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Nutrition_Light);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Nutrition_Filling);

#pragma endregion
	
#pragma region Food Flavour
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Sweet);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Sour);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Salty);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Bitter);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Spicy);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Savory);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Smoky);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Fresh);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Bland);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Flavour_Disgusting);
	
#pragma endregion
	
#pragma region Food Occasion

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Occasion);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Occasion_DailyMeal);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Occasion_Feast);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Occasion_Snack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Occasion_Dessert);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Food_Occasion_Celebration);

#pragma endregion
	
#pragma region Drink

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Drink_Type);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Drink_Type_Water);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Drink_Type_Juice);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Drink_Type_Alcohol);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Drink_Type_Tea);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Drink_Type_Coffee);

#pragma endregion
	
#pragma endregion
	
#pragma region Purchasable Economics

	//Rarity
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Rarity_Common);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Rarity_Uncommon);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Rarity_Rare);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Rarity_ExtremelyRare);
	
	//Luxury Levels
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_LuxuryLevel_Cheap);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_LuxuryLevel_Affordable);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_LuxuryLevel_Premium);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_LuxuryLevel_Luxury);
	
#pragma endregion
	
#pragma region Purchasable Effects

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Effects);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Social_StatusSymbol);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Social_Giftable);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Social_Comfort);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Purchasable_Social_Entertainment);

#pragma endregion
	
#pragma endregion
	
}
