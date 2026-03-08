#pragma once
#include "HealthConditionDataAsset.h"
#include "CustomerHealthCondition.generated.h"

USTRUCT(BlueprintType)
struct FCustomerHealthCondition
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="General")
	FName HealthConditionName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<const UHealthConditionDataAsset> Definition;

	// 0–100, 0 = healed, 100 = transition to more severe condition
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Severity = 1.f;

	// How long the condition has existed (hours)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DurationHours = 0.f;
	

	bool IsValid() const
	{
		return Definition != nullptr;
	}

	float GetHealthImpact() const
	{
		if (!Definition)
			return 0.f;

		return Definition->HealthPenalty * Severity;
	}
	
	void UpdateHealthCondition()
	{
		// ToDo Use daily worsening and daily recovery chance to update severity
		// ToDo Proceed to more severe condition if high severity
	}
	
	void EffectOfPurchasable(FGameplayTagContainer Tags)
	{
		// ToDo Check if Tags are in trigger or healing tags
		// ToDo If yes, impact on severity
	}
	
	//Has the condition become deathly
	bool Deathly()
	{
		return false;
	}
};
