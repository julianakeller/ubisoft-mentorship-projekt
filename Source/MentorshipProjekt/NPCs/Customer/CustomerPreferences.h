#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MentorshipProjekt/Purchasables/PurchasableDefinition.h"
#include "CustomerPreferences.generated.h"

USTRUCT(BlueprintType)
struct FCustomerPreferences
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customer Preferences", meta=(Categories="Purchasable"))
	FGameplayTagContainer PreferredTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customer Preferences")
	TArray<UPurchasableDefinition*> PreferredPurchasables;
};
