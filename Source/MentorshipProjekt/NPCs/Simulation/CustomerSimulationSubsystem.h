// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCSimulationSubsystem.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerInstanceData.h"
#include "Subsystems/WorldSubsystem.h"
#include "MentorshipProjekt/NPCs/Customer/Household.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerRelationship.h"
#include "CustomerSimulationSubsystem.generated.h"

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
	//Trigger visit scheduling
	//Update preferences
	//Background life simulation
	
public:
	
	// ---------Customer spawning settings:---------

	UPROPERTY(EditAnywhere)
	float BaseSpawnChance = 1.f;
	
	// Chance Increase of spawning new customer per in game hour
	UPROPERTY(EditAnywhere)
	float BaseSpawnChanceIncrease = 0.25f;
	
	// Damping factor controls how strongly existing customers slow growth
	UPROPERTY(EditAnywhere)
	float DampingFactor = 0.5f; 
	
	UPROPERTY(EditAnywhere)
	int32 MaximumCustomerCount = 400;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void OnMinuteChanged(const FInGameTime& NewTime) override;
	void OnHourChanged(const FInGameTime& NewTime) override;
	void OnDayChanged(const FInGameTime& NewTime) override;
	void OnWeekChanged(const FInGameTime& NewTime) override;
	void OnMonthChanged(const FInGameTime& NewTime) override;
	
	//ToDo Add function: Modify Customer Funds (by customer id)
	//ToDo Add function: Set customer funds
	
	FHousehold* GetHouseholdById(const FGuid& Id);
	
private:
	
	// --------Instace Maps:------------
	UPROPERTY()
	TMap<FGuid, FCustomerInstanceData> Customers;
	
	UPROPERTY()
	TMap<FGuid, FHousehold> Households;
	
	// ------------Data Assets for selecting random Customer Stats:----------
	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<UProfessionDataAsset>> ProfessionDataAssets;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<UHealthConditionDataAsset>> HealthConditionDataAssets;
	
	UPROPERTY(EditDefaultsOnly, Category="Relationships")
	TSoftObjectPtr<URelationshipRuleDataAsset> RelationshipRulesAsset;
	
	TMap<ERelationshipTypes, TArray<FRelationshipTransitionRule>> RulesByType;
	
	// ---------Functions for initializing data assets:
	
	void LoadCustomerDataAssets();
	
	void BuildTransitionMap();
	
	// -----------Functions for simulating customers:
	
	void UpdateCustomersHourly();
	void UpdateCustomersDaily();
	void UpdateCustomersWeekly();
	void UpdateCustomersMonthly();
	
	//Update age
	void UpdateCustomerAge(FCustomerInstanceData& Customer, FInGameTime NewTime);
	
	//Update funds
	void UpdateCustomerFunds(FCustomerInstanceData& Customer);
	
	//Update inventory
	void SimulateCustomerInventory(FCustomerInstanceData& Customer);
	
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
	void ScheduleVisit(FCustomerInstanceData& Customer);
	
	// -------------Functions for creating customers:
	void CreateCustomerInstance();
	
	UFUNCTION()
	void GenerateNewCustomer(const FInGameTime& NewTime, FInGameTime& LastUpdate);
	
	void GenerateChildCustomer(FCustomerInstanceData& ParentCustomer);
	
	TArray<FGameplayTag> GenerateNewCustomerPreferences(FCustomerInstanceData& Customer, int32 NumberOfTagsToAdd = 1);
	
	void SelectNewCustomerProfession(FCustomerInstanceData& Customer);
	
	// ------------Hosuehold functions:------------
	
	void ModifyHouseholdFunds(const FGuid* Id, float FundsToAdd);
	
	float GetHouseholdFunds(const FGuid* Id);
	
	// --------------Relationship functions:
	
	FCustomerRelationship* GetOrCreateRelationship(FCustomerInstanceData& CustomerA, FCustomerInstanceData& CustomerB);
	
	// ---------------Other:------------------
	
	UFUNCTION(BlueprintCallable, Category="Debug")
	void LogAllCustomerData() const;
	
	bool bHasInitializedTime = false;
};
