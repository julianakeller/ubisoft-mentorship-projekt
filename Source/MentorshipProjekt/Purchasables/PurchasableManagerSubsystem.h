// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "GameTimeSubsystem.h"
#include "PurchasableDefinition.h" 
#include "PurchasableQuality.h"
#include "PurchasableFreshness.h" 
#include "PurchasableInstance.h"
#include "InventoryStack.h"
#include "PurchasableManagerSubsystem.generated.h"

/**
 * 
 */

class UGameTimeSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

UCLASS()
class MENTORSHIPPROJEKT_API UPurchasableManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	UPurchasableInstance* ProducePurchasable(UPurchasableDefinition* Definition, int32 WorkerSkillLevel);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool BuyPurchasable(UPurchasableDefinition* Definition, int32 Count);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	EPurchasableFreshness GetFreshnessLevel(const float Freshness);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	EPurchasableQuality GetQualityLevel(const float Quality);
	
protected:
	
	TArray<FInventoryStack> GetAllStacks() const;
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	float GetTotalInventoryValue() const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetPurchasableCountByDefinition(UPurchasableDefinition* Definition) const;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged OnInventoryChanged;
	
private:

	UPROPERTY()
	 TMap<FInventoryStackKey, FInventoryStack> Inventory;

	UPROPERTY()
	TObjectPtr<UGameTimeSubsystem> TimeSubsystem;

	UFUNCTION()
	void HandleHourChanged(const FInGameTime& NewTime);

	void UpdateStackFreshness(FInventoryStack& Stack, FInGameTime CurrentTime) const;

	void RemoveExpiredItems();
};
