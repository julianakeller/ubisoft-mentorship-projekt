// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAssetManager.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

#include "MentorshipProjekt/NPCs/Customer/ProfessionDataAsset.h"
#include "MentorshipProjekt/Purchasables/RecipeDefinition.h"
#include "MentorshipProjekt/Purchasables/PurchasableDefinition.h"
#include "MentorshipProjekt/NPCs/FamilyMemberDataAsset.h"
#include "MentorshipProjekt/NPCs/Customer/RelationshipRuleDataAsset.h"
#include "MentorshipProjekt/NPCs/Customer/HealthConditionDataAsset.h"
#include "MentorshipProjekt/NPCs/Skills/SkillDataAsset.h"

void UMyAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();

    // Preload all asset types
    TArray<FPrimaryAssetId> AssetIds;

    // Professions
    GetPrimaryAssetIdList(ProfessionAssetType, AssetIds);
    for (const FPrimaryAssetId& Id : AssetIds)
    {
        if (UProfessionDataAsset* Asset = Cast<UProfessionDataAsset>(GetPrimaryAssetObject(Id)))
        {
            ProfessionAssets.Add(Id.PrimaryAssetName, Asset);
            UE_LOG(LogTemp, Log, TEXT("Preloaded Profession: %s"), *Id.PrimaryAssetName.ToString());
        }
    }
    AssetIds.Reset();

    // Recipes
    GetPrimaryAssetIdList(RecipeAssetType, AssetIds);
    for (const FPrimaryAssetId& Id : AssetIds)
    {
        if (URecipeDefinition* Asset = Cast<URecipeDefinition>(GetPrimaryAssetObject(Id)))
        {
            RecipeAssets.Add(Id.PrimaryAssetName, Asset);
            UE_LOG(LogTemp, Log, TEXT("Preloaded Recipe: %s"), *Id.PrimaryAssetName.ToString());
        }
    }
    AssetIds.Reset();

    // Purchasables
    GetPrimaryAssetIdList(PurchasableAssetType, AssetIds);
    for (const FPrimaryAssetId& Id : AssetIds)
    {
        if (UPurchasableDefinition* Asset = Cast<UPurchasableDefinition>(GetPrimaryAssetObject(Id)))
        {
            PurchasableAssets.Add(Id.PrimaryAssetName, Asset);
            UE_LOG(LogTemp, Log, TEXT("Preloaded Purchasable: %s"), *Id.PrimaryAssetName.ToString());
        }
    }
    AssetIds.Reset();

    // Family Members
    GetPrimaryAssetIdList(FamilyMemberAssetType, AssetIds);
    for (const FPrimaryAssetId& Id : AssetIds)
    {
        if (UFamilyMemberDataAsset* Asset = Cast<UFamilyMemberDataAsset>(GetPrimaryAssetObject(Id)))
        {
            FamilyMemberAssets.Add(Id.PrimaryAssetName, Asset);
            UE_LOG(LogTemp, Log, TEXT("Preloaded Family Member: %s"), *Id.PrimaryAssetName.ToString());
        }
    }
    AssetIds.Reset();

    // Relationship Rules
    GetPrimaryAssetIdList(RelationshipRuleAssetType, AssetIds);
    for (const FPrimaryAssetId& Id : AssetIds)
    {
        if (URelationshipRuleDataAsset* Asset = Cast<URelationshipRuleDataAsset>(GetPrimaryAssetObject(Id)))
        {
            RelationshipRuleAssets.Add(Id.PrimaryAssetName, Asset);
            UE_LOG(LogTemp, Log, TEXT("Preloaded Relationship Rule: %s"), *Id.PrimaryAssetName.ToString());
        }
    }
    AssetIds.Reset();

    // Health Conditions
    GetPrimaryAssetIdList(HealthConditionAssetType, AssetIds);
    for (const FPrimaryAssetId& Id : AssetIds)
    {
        if (UHealthConditionDataAsset* Asset = Cast<UHealthConditionDataAsset>(GetPrimaryAssetObject(Id)))
        {
            HealthConditionAssets.Add(Id.PrimaryAssetName, Asset);
            UE_LOG(LogTemp, Log, TEXT("Preloaded Health Condition: %s"), *Id.PrimaryAssetName.ToString());
        }
    }
    AssetIds.Reset();

    // Skills
    GetPrimaryAssetIdList(SkillAssetType, AssetIds);
    for (const FPrimaryAssetId& Id : AssetIds)
    {
        if (USkillDataAsset* Asset = Cast<USkillDataAsset>(GetPrimaryAssetObject(Id)))
        {
            SkillAssets.Add(Id.PrimaryAssetName, Asset);
            UE_LOG(LogTemp, Log, TEXT("Preloaded Skill: %s"), *Id.PrimaryAssetName.ToString());
        }
    }
    AssetIds.Reset();
}

#pragma region Getters
UProfessionDataAsset* UMyAssetManager::GetProfessionAsset(const FName AssetName) const
{
    return ProfessionAssets.FindRef(AssetName);
}

URecipeDefinition* UMyAssetManager::GetRecipeAsset(const FName AssetName) const
{
    return RecipeAssets.FindRef(AssetName);
}

UPurchasableDefinition* UMyAssetManager::GetPurchasableAsset(const FName AssetName) const
{
    return PurchasableAssets.FindRef(AssetName);
}

UFamilyMemberDataAsset* UMyAssetManager::GetFamilyMemberAsset(const FName AssetName) const
{
    return FamilyMemberAssets.FindRef(AssetName);
}

URelationshipRuleDataAsset* UMyAssetManager::GetRelationshipRuleAsset(const FName AssetName) const
{
    return RelationshipRuleAssets.FindRef(AssetName);
}

UHealthConditionDataAsset* UMyAssetManager::GetHealthConditionAsset(const FName AssetName) const
{
    return HealthConditionAssets.FindRef(AssetName);
}

USkillDataAsset* UMyAssetManager::GetSkillAsset(const FName AssetName) const
{
    return SkillAssets.FindRef(AssetName);
}

#pragma endregion 