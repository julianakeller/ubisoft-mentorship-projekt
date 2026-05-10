// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "MentorshipProjekt/Purchasables/PurchasableDefinition.h"
#include "InteractableWorkstationComponent.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWorkstationProductionSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPurchasableDefinition* Purchasable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetAmount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority = 0; // 0 = default, higher = higher priority
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnProductionSettingsChanged, UInteractableWorkstationComponent*);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UInteractableWorkstationComponent : public UInteractableComponentBase
{
	GENERATED_BODY()
	
public:
	
	UInteractableWorkstationComponent();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Workstation")
	FName WorkstationName;

	// What this workstation can produce
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Workstation")
	TArray<TObjectPtr<UPurchasableDefinition>> AvailablePurchasables;
	
	UPROPERTY()
	TArray<int32> SortedSettingIndices;
	
	FOnProductionSettingsChanged OnProductionSettingsChanged;
	
	const TArray<FWorkstationProductionSetting>& GetProductionSettings() const {return ProductionSettings;}
	
	// Return const copy so that it cannot be accidentally modified
	const FWorkstationProductionSetting* GetProductionSetting(int32 Index) const;
	
	// Change the setting with the same purchasable definition to the settings of the passed const Setting
	void SetProductionSetting(const FWorkstationProductionSetting Setting);
	
	const FWorkstationProductionSetting* GetHighestPriorityEnabledSetting() const;
	
	UPurchasableDefinition* GetHighestPriorityPurchasable();
	
	const FWorkstationProductionSetting* GetSettingSorted(int32 SortedIndex) const;
	
	void RebuildSortedIndices();
	
	void NotifyProductionSettingsChanged();
	
private:
	
	// Current production configuration
	TArray<FWorkstationProductionSetting> ProductionSettings;
};
