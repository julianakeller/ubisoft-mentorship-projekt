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

	// Current production configuration
	UPROPERTY(BlueprintReadOnly, Category="Workstation")
	TArray<FWorkstationProductionSetting> ProductionSettings;
	
	FWorkstationProductionSetting* GetHighestPriorityEnabledSetting();
	
	UFUNCTION(BlueprintCallable)
	void SortProductionSettings();
	
	FWorkstationProductionSetting* GetSetting(int32 Index);
};
