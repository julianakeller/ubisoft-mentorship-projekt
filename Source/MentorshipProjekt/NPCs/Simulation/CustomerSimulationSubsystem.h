// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCSimulationSubsystem.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerInstanceData.h"
#include "Subsystems/WorldSubsystem.h"
#include "MentorshipProjekt/NPCs/Customer/Household.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerRelationship.h"
#include "CustomerSimulationSubsystem.generated.h"

class ACustomerCharacter;
class ULogSubsystem;
class URelationshipRuleDataAsset;
struct FInGameTime;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UCustomerSimulationSubsystem : public UNPCSimulationSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	// ---------Customer spawning settings:------------

	UPROPERTY(EditAnywhere)
	float BaseSpawnChance = 0.05f;
	
	// Chance Increase of spawning new customer per in game hour
	UPROPERTY(EditAnywhere)
	float BaseSpawnChanceIncrease = 0.01f;
	
	// Damping factor controls how strongly existing customers slow growth
	UPROPERTY(EditAnywhere)
	float DampingFactor = 1.f; 
	
	UPROPERTY(EditAnywhere)
	int32 MaximumCustomerCount = 400;
	
	void DespawnCustomer(const FGuid& CustomerId);

	// ---------Time Change Handlers:------------
	
	virtual void OnMinuteChanged(const FInGameTime& NewTime) override;
	virtual void OnHourChanged(const FInGameTime& NewTime) override;
	virtual void OnDayChanged(const FInGameTime& NewTime) override;
	virtual void OnWeekChanged(const FInGameTime& NewTime) override;
	virtual void OnMonthChanged(const FInGameTime& NewTime) override;
	
	FHousehold* GetHouseholdById(const FGuid& Id);
	
	// --------Instace Maps:------------
	UPROPERTY()
	TMap<FGuid, FCustomerInstanceData> Customers;
	
	UPROPERTY()
	TMap<FGuid, FHousehold> Households;
	
	// ------------Hosuehold functions:------------
	
	void UpdateHouseholdInventory(FHousehold& Household);
	
	void UpdateHouseholdShoppingList(FHousehold& Household);
	
	void ModifyHouseholdFunds(const FGuid& HouseholdId, float FundsToAdd);
	
	float GetHouseholdFunds(const FGuid& Id);
	
	void AddPurchasablesToHousehold(const FGuid& HouseholdId, const TArray<UPurchasableInstance*>& Purchasables);
	
	TMap<FGameplayTag, FDesiredTagEntry>* GetHouseholdShoppingList(const FGuid& HouseholdId);
	
	TMap<FGameplayTag, FDesiredTagEntry>* GetCustomerShoppingList(const FGuid& CustomerId);
	
private:
	
	// ------------Data Assets for selecting random Customer Stats:------------
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<UProfessionDataAsset>> ProfessionDataAssets;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<UHealthConditionDataAsset>> HealthConditionDataAssets;
	
	UPROPERTY(EditDefaultsOnly, Category="Relationships")
	TSoftObjectPtr<URelationshipRuleDataAsset> RelationshipRulesAsset;
	
	TMap<ERelationshipTypes, TArray<FRelationshipTransitionRule>> RulesByType;
	
	// ------------For Spawning Customers in Level:------------
	UPROPERTY()
	TSoftClassPtr<ACustomerCharacter> CustomerCharacterSoftClass;

	UPROPERTY()
	TSubclassOf<ACustomerCharacter> CustomerCharacterClass;

	UPROPERTY()
	TArray<AActor*> CustomerSpawnPoints;
	
	void LoadCustomerCharacterClass();
	
	void CacheSpawnPoints(UWorld* World);
	
	int32 SpawnedCustomersCount = 0;
	
	// ---------Functions for initializing data assets:------------
	
	void LoadCustomerDataAssets();
	
	void BuildTransitionMap();
	
	// -----------Functions for simulating customers:------------
	
	//Update age
	void UpdateCustomerAge(FCustomerInstanceData& Customer, FInGameTime NewTime);
	
	// Update health
	void SimulateHealth(FCustomerInstanceData& Customer, FInGameTime NewTime);
	
	//Update relationships
	void SimulateCustomerRelationships(FCustomerInstanceData& Customer);
	void SimulateRelationship(FCustomerInstanceData& CustomerA, FCustomerInstanceData& CustomerB, TSharedPtr<FCustomerRelationship> Rel = nullptr);
	
	//Update preferences
	void UpdateCustomerPreferences(FCustomerInstanceData& Customer);
	
	//Update customer profession
	void SimulateProfessionChanges(FCustomerInstanceData& Customer);
	
	//Schedule next visit
	void ScheduleVisit();
	
	// -------------Functions for creating customers:------------
	
	void CreateCustomerInstance();
	
	UFUNCTION()
	void GenerateNewCustomer(const FInGameTime& NewTime, FInGameTime& LastUpdate);
	
	void GenerateChildCustomer(FCustomerInstanceData& ParentCustomer);
	
	TArray<FGameplayTag> GenerateNewCustomerPreferences(FCustomerInstanceData& Customer, int32 NumberOfTagsToAdd = 1);
	
	void SelectNewCustomerProfession(FCustomerInstanceData& Customer);
	
	// --------------Relationship functions:------------
	
	FCustomerRelationship* GetOrCreateRelationship(FCustomerInstanceData& CustomerA, FCustomerInstanceData& CustomerB);
	
	// ---------------Other:------------------
	
	UFUNCTION(BlueprintCallable, Category="Debug")
	void LogAllCustomerData() const;
	
	bool bHasInitializedTime = false;
};
