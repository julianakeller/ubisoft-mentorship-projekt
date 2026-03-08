// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MyAssetManager.generated.h"

/**
 * 
 */

class UProfessionDataAsset;
class URecipeDefinition;
class UPurchasableDefinition;
class UFamilyMemberDataAsset;
class URelationshipRuleDataAsset;
class UHealthConditionDataAsset;
class USkillDataAsset;

UCLASS()
class MENTORSHIPPROJEKT_API UMyAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	virtual void StartInitialLoading() override;

	/** Convenience getters */
	UProfessionDataAsset* GetProfessionAsset(const FName AssetName) const;
	URecipeDefinition* GetRecipeAsset(const FName AssetName) const;
	UPurchasableDefinition* GetPurchasableAsset(const FName AssetName) const;
	UFamilyMemberDataAsset* GetFamilyMemberAsset(const FName AssetName) const;
	URelationshipRuleDataAsset* GetRelationshipRuleAsset(const FName AssetName) const;
	UHealthConditionDataAsset* GetHealthConditionAsset(const FName AssetName) const;
	USkillDataAsset* GetSkillAsset(const FName AssetName) const;

private:

	void LoadAllAssetsOfType(TArray<FPrimaryAssetId>& OutIds);

	TMap<FName, UProfessionDataAsset*> ProfessionAssets;
	TMap<FName, URecipeDefinition*> RecipeAssets;
	TMap<FName, UPurchasableDefinition*> PurchasableAssets;
	TMap<FName, UFamilyMemberDataAsset*> FamilyMemberAssets;
	TMap<FName, URelationshipRuleDataAsset*> RelationshipRuleAssets;
	TMap<FName, UHealthConditionDataAsset*> HealthConditionAssets;
	TMap<FName, USkillDataAsset*> SkillAssets;

	const FPrimaryAssetType ProfessionAssetType = FPrimaryAssetType("Profession");
	const FPrimaryAssetType RecipeAssetType = FPrimaryAssetType("Recipe");
	const FPrimaryAssetType PurchasableAssetType = FPrimaryAssetType("Purchasable");
	const FPrimaryAssetType FamilyMemberAssetType = FPrimaryAssetType("FamilyMember");
	const FPrimaryAssetType RelationshipRuleAssetType = FPrimaryAssetType("RelationshipRule");
	const FPrimaryAssetType HealthConditionAssetType = FPrimaryAssetType("HealthCondition");
	const FPrimaryAssetType SkillAssetType = FPrimaryAssetType("Skill");
};
