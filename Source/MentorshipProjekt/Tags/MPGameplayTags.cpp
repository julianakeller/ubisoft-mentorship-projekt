#include "MPGameplayTags.h"

namespace MPGameplayTags
{
	
#pragma region Actions
	
	// Actions Root
	UE_DEFINE_GAMEPLAY_TAG(TAG_Action, "Action");

	// Food-related actions
	UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Cooking, "Action.Cooking");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Action_PreparingFood, "Action.PreparingFood");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Bartending, "Action.Bartending");

	// Farming actions
	UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Sowing, "Action.Sowing");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Harvesting, "Action.Harvesting");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Action_Watering, "Action.Watering");

	// Production / workstation actions
	UE_DEFINE_GAMEPLAY_TAG(TAG_Action_FillingBarrel, "Action.FillingBarrel");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Action_TendingCounter, "Action.TendingCounter");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Action_ServingCustomer, "Action.ServingCustomer");
	
#pragma endregion 
	
#pragma region Skills
	
	// Skills Root
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill, "Skill");

	// Individual Skills
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Gardening, "Skill.Gardening");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Cooking, "Skill.Cooking");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Fermenting, "Skill.Fermenting");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Bartending, "Skill.Bartending");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Medicine, "Skill.Medicine");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Chemistry, "Skill.Chemistry");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Teaching, "Skill.Teaching");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Crafting, "Skill.Crafting");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Bartering, "Skill.Bartering");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_AnimalHandling, "Skill.AnimalHandling");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Baking, "Skill.Baking");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Painting, "Skill.Painting");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Sculpting, "Skill.Sculpting");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Skill_Sewing, "Skill.Sewing");
	
#pragma endregion
	
#pragma region Purchasables

// Purchasable Root
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable, "Purchasable");

#pragma region Consumable

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Consumable, "Purchasable.Consumable");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Consumable_Edible, "Purchasable.Consumable.Edible");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Consumable_Drinkable, "Purchasable.Consumable.Drinkable");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Consumable_Healing, "Purchasable.Consumable.Healing");

#pragma endregion

#pragma region Categories

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food, "Purchasable.Food");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Drink, "Purchasable.Drink");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Medicine, "Purchasable.Medicine");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Cosmetic, "Purchasable.Cosmetic");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Clothing, "Purchasable.Clothing");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Tool, "Purchasable.Tool");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Furniture, "Purchasable.Furniture");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Material, "Purchasable.Material");

#pragma endregion

#pragma region Medicine

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Medicine_Effect, "Purchasable.Medicine.Effect");

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Medicine_Effect_Healing, "Purchasable.Medicine.Effect.Healing");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Medicine_Effect_Energy, "Purchasable.Medicine.Effect.Energy");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Medicine_Effect_Calm, "Purchasable.Medicine.Effect.Calm");

#pragma endregion

#pragma region Cosmetic

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Cosmetic_Type, "Purchasable.Cosmetic.Type");

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Cosmetic_Type_Perfume, "Purchasable.Cosmetic.Type.Perfume");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Cosmetic_Type_Soap, "Purchasable.Cosmetic.Type.Soap");

#pragma endregion

#pragma region Food

#pragma region Food Type

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Vegetable, "Purchasable.Food.Type.Vegetable");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Fruit, "Purchasable.Food.Type.Fruit");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Meat, "Purchasable.Food.Type.Meat");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Fish, "Purchasable.Food.Type.Fish");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Grain, "Purchasable.Food.Type.Grain");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Dairy, "Purchasable.Food.Type.Dairy");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Herb, "Purchasable.Food.Type.Herb");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Spice, "Purchasable.Food.Type.Spice");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Bread, "Purchasable.Food.Type.Bread");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Dessert, "Purchasable.Food.Type.Dessert");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Soup, "Purchasable.Food.Type.Soup");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Stew, "Purchasable.Food.Type.Stew");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Type_Preserved, "Purchasable.Food.Type.Preserved");

#pragma endregion

#pragma region Food Nutrition

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Nutrition, "Purchasable.Food.Nutrition");

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Nutrition_Healthy, "Purchasable.Food.Nutrition.Healthy");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Nutrition_Unhealthy, "Purchasable.Food.Nutrition.Unhealthy");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Nutrition_HighProtein, "Purchasable.Food.Nutrition.HighProtein");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Nutrition_HighEnergy, "Purchasable.Food.Nutrition.HighEnergy");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Nutrition_Light, "Purchasable.Food.Nutrition.Light");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Nutrition_Filling, "Purchasable.Food.Nutrition.Filling");

#pragma endregion

#pragma region Food Flavour

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Sweet, "Purchasable.Food.Flavour.Sweet");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Sour, "Purchasable.Food.Flavour.Sour");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Salty, "Purchasable.Food.Flavour.Salty");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Bitter, "Purchasable.Food.Flavour.Bitter");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Spicy, "Purchasable.Food.Flavour.Spicy");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Savory, "Purchasable.Food.Flavour.Savory");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Smoky, "Purchasable.Food.Flavour.Smoky");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Fresh, "Purchasable.Food.Flavour.Fresh");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Bland, "Purchasable.Food.Flavour.Bland");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Flavour_Disgusting, "Purchasable.Food.Flavour.Disgusting");

#pragma endregion

#pragma region Food Occasion

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Occasion, "Purchasable.Food.Occasion");

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Occasion_DailyMeal, "Purchasable.Food.Occasion.DailyMeal");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Occasion_Feast, "Purchasable.Food.Occasion.Feast");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Occasion_Snack, "Purchasable.Food.Occasion.Snack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Occasion_Dessert, "Purchasable.Food.Occasion.Dessert");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Food_Occasion_Celebration, "Purchasable.Food.Occasion.Celebration");

#pragma endregion

#pragma region Drink

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Drink_Type, "Purchasable.Drink.Type");

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Drink_Type_Water, "Purchasable.Drink.Type.Water");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Drink_Type_Juice, "Purchasable.Drink.Type.Juice");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Drink_Type_Alcohol, "Purchasable.Drink.Type.Alcohol");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Drink_Type_Tea, "Purchasable.Drink.Type.Tea");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Drink_Type_Coffee, "Purchasable.Drink.Type.Coffee");

#pragma endregion

#pragma endregion

#pragma region Purchasable Economics

// Rarity
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Rarity_Common, "Purchasable.Rarity.Common");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Rarity_Uncommon, "Purchasable.Rarity.Uncommon");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Rarity_Rare, "Purchasable.Rarity.Rare");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Rarity_ExtremelyRare, "Purchasable.Rarity.ExtremelyRare");

// Luxury Levels
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_LuxuryLevel_Cheap, "Purchasable.LuxuryLevel.Cheap");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_LuxuryLevel_Affordable, "Purchasable.LuxuryLevel.Affordable");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_LuxuryLevel_Premium, "Purchasable.LuxuryLevel.Premium");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_LuxuryLevel_Luxury, "Purchasable.LuxuryLevel.Luxury");

#pragma endregion

#pragma region Purchasable Effects

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Effects, "Purchasable.Effects");

UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Social_StatusSymbol, "Purchasable.Social.StatusSymbol");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Social_Giftable, "Purchasable.Social.Giftable");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Social_Comfort, "Purchasable.Social.Comfort");
UE_DEFINE_GAMEPLAY_TAG(TAG_Purchasable_Social_Entertainment, "Purchasable.Social.Entertainment");

#pragma endregion

#pragma endregion
	
}