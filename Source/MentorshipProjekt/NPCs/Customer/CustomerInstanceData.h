// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomerDataAsset.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerPreferences.h"
#include "CustomerRelationship.h"
#include "Household.h"
#include "../NPCInstanceData.h"
#include "CustomerDefaults.h"
#include "CustomerHealthCondition.h"
#include "CustomerProfession.h"
#include "CustomerInstanceData.generated.h"

struct FCustomerPreferences;
class UCustomerSimulationSubsystem;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FCustomerInstanceData : public FNPCInstanceData
{
	GENERATED_BODY()
	
	//Dynamic customer stats
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Customer")
	FName CustomerName;
	
	UPROPERTY()
	FGuid CustomerId;
	
	UPROPERTY()
	FGuid HouseholdId;
	
	//Key = other customer ID
	TMap<FGuid, TSharedPtr<FCustomerRelationship>> Relationships;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	TArray<FCustomerHealthCondition> HealthConditions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Economy")
	FCustomerProfession Profession;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Customer")
	bool bFemale = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customer")
	float Age = 25.f;
	
	// 0-100
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customer")
	float Health = 85.f;
	
	// 0-100
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customer")
	float Goodwill = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customer")
	int32 VisitCount = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Data")
	FCustomerPreferences Preferences;
	
	void InitializeFromDataAsset()
	{
		CustomerId = FGuid::NewGuid();
		
		bFemale = FMath::RandRange(0, 1) > 0.5f;
		
		// Random Name:
		// First name:
		FString FirstName;
		if (bFemale)
		{
			int32 Index = FMath::RandRange(0, FCustomerDefaults::FemaleFirstNames.Num() - 1);
			FirstName = FCustomerDefaults::FemaleFirstNames[Index];
		}
		else
		{
			int32 Index = FMath::RandRange(0, FCustomerDefaults::MaleFirstNames.Num() - 1);
			FirstName = FCustomerDefaults::MaleFirstNames[Index];
		}
		
		// Last name:
		int32 LastNameIndex = FMath::RandRange(0, FCustomerDefaults::LastNames.Num() - 1);
		FString LastName = FCustomerDefaults::LastNames[LastNameIndex];
		
		// Combine into full name:
		FString FullName = FirstName + TEXT(" ") + LastName;
		CustomerName = FName(*FullName);
		
		// Default stats:
		Goodwill = 50.f;
		VisitCount = 0;
		
		//Random age:
		const float MinAge = FCustomerDefaults::MinAge;
		const float MaxAge = FMath::Max(MinAge, FCustomerDefaults::MaxAge);
		Age = FMath::RandRange(MinAge, MaxAge);

		ClampCustomerStats();
	}
	
	void ClampCustomerStats()
	{
		Goodwill = FMath::Clamp(Goodwill, 0.f, 100.f);
	}
	
	void ApplyVisitOutcome(const float OutcomeValue)
	{
		Goodwill = FMath::Clamp(Goodwill + OutcomeValue, 0.f, 100.f);
		VisitCount++;
	}
	
	float GetPriceToleranceMultiplier() const
	{
		// Goodwill 0–100 -> tolerance 0.5–1.5
		return FMath::GetMappedRangeValueClamped(FVector2D(0.f, 100.f), FVector2D(0.5f, 1.5f), Goodwill);
	}
};
