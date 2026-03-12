// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "MentorshipProjekt/Purchasables/PurchasableManagerSubsystem.h"
#include "PlayerProductionComponent.generated.h"

struct FWorkstationProductionSetting;
class UGameTimeSubsystem;
class UInteractableWorkstationComponent;
/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UPlayerProductionComponent : public UInteractableComponentBase
{
	GENERATED_BODY()
	
public:
	UPlayerProductionComponent();
	
	void StartProduction();

	void StopProduction();
	
protected:
	
	virtual void BeginPlay() override;
	
	//Triggered by Q (secondary interaction button)
	virtual void OnInteractSecondary(AActor* Interactor) override;

	UPROPERTY()
	UInteractableWorkstationComponent* ProductionWorkstation;

	UPROPERTY()
	AActor* ProducingPlayer;

	FTimerHandle ProductionTimerHandle;

	void ProduceNextItem();

	void OnProductionComplete();

	bool bIsProducing = false;
	
private:
	
	// Current workstation setting being produced
	FWorkstationProductionSetting* CurrentSetting = nullptr;
	
	UPROPERTY()
	UGameTimeSubsystem* GameTimeSubsystem;

	UPROPERTY()
	UPurchasableManagerSubsystem* PurchasableManager;
	
	URecipeDefinition* Recipe = nullptr;
};
