#pragma once

#include "CoreMinimal.h"
#include "CustomerRelationship.generated.h"

UENUM(BlueprintType)
enum class ERelationshipTypes : uint8
{
	// No connection
	None				UMETA(DisplayName="None"),

	// Social
	Acquaintance		UMETA(DisplayName="Acquaintance"),
	Friend				UMETA(DisplayName="Friend"),
	CloseFriend			UMETA(DisplayName="Close Friend"),
	Rival				UMETA(DisplayName="Rival"),

	// Romantic
	Crush				UMETA(DisplayName="Crush"),
	Partner				UMETA(DisplayName="Partner"),
	Spouse				UMETA(DisplayName="Spouse"),
	ExPartner			UMETA(DisplayName="Ex Partner"),

	// Family – Direct
	Parent				UMETA(DisplayName="Parent"),
	Child				UMETA(DisplayName="Child"),
	Sibling				UMETA(DisplayName="Sibling"),

	// Family – Extended
	Grandparent			UMETA(DisplayName="Grandparent"),
	Grandchild			UMETA(DisplayName="Grandchild"),
	OtherRelative		UMETA(DisplayName="Other Relative")
};

USTRUCT(BlueprintType)
struct FRelationshipTransitionRule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERelationshipTypes From = ERelationshipTypes::None;

	UPROPERTY(EditAnywhere)
	ERelationshipTypes To = ERelationshipTypes::None;

	UPROPERTY(EditAnywhere)
	float MinStrengthForTransition = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxStrengthForTransition = 100.f;
	
	UPROPERTY(EditAnywhere)
	float NewStrength = 50.f;

	UPROPERTY(EditAnywhere)
	float Probability = 1.f;
	
	UPROPERTY(EditAnywhere)
	bool bPregnancyChance = false;
	
	UPROPERTY(EditAnywhere)
	bool bUpdateOtherCustomer = true;
	
	UPROPERTY(EditAnywhere)
	bool bUpdateHousehold = false;
};

USTRUCT(BlueprintType)
struct FCustomerRelationship
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid CustomerAId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid CustomerBId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERelationshipTypes Type = ERelationshipTypes::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength = 50.f; //-100 to 100
	
	//Add a positive or negative value to Relationship-Strength
	void AddStrength(float StrengthChange) 
	{
		Strength = FMath::Clamp(Strength + StrengthChange, -100.f, 100.f);
	}
	
	//Returns the applied FRelationshipTransitionRule or nullptr
	const FRelationshipTransitionRule* AdvanceRelationship(const float InteractionQuality, const TMap<ERelationshipTypes, TArray<FRelationshipTransitionRule>>& RulesByType)
	{
		AddStrength(InteractionQuality * 0.1f);
		
		// Lookup applicable rules for current type
		const TArray<FRelationshipTransitionRule>* ApplicableRules = RulesByType.Find(Type);
		if (!ApplicableRules || ApplicableRules->Num() == 0)
		{
			return nullptr;
		}
		
		for (const FRelationshipTransitionRule& Rule : *ApplicableRules)
		{
			if (Rule.To == ERelationshipTypes::None)
			{
				continue;
			}
			if (Strength < Rule.MinStrengthForTransition || Strength > Rule.MaxStrengthForTransition)
			{
				continue;
			}
			
			if (FMath::FRand() > Rule.Probability)
			{
				continue;
			}
			
			//Apply relationship transition:
			Type = Rule.To;
			Strength = Rule.NewStrength;
			
			return &Rule; //Return applied rule so that customer manager can adjust involved customer
		}
		return nullptr;
	}
};
