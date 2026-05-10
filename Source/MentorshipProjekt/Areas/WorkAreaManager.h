// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MentorshipProjekt/InteractionSystem/InteractableComponents/InteractableWorkstationComponent.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorkAreaManager.generated.h"

USTRUCT(BlueprintType)
struct FTaggedWorkstationEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UInteractableWorkstationComponent> Workstation = nullptr;

	UPROPERTY()
	TObjectPtr<UPurchasableDefinition> Purchasable = nullptr;
	
	// UPROPERTY()
	// int32 AmountAvailable = 0;
};

class AWorkAreaBase;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UWorkAreaManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void RegisterWorkArea(AWorkAreaBase* Area);
	void UnregisterWorkArea(AWorkAreaBase* Area);

	const TArray<AWorkAreaBase*>& GetAllWorkAreas() const;

	const AWorkAreaBase* GetWorkAreaByName(FName Name) const;
	
	const TArray<FTaggedWorkstationEntry>* GetEntriesForTag(const FGameplayTag& Tag) const;
	
private:

	UPROPERTY()
	TArray<AWorkAreaBase*> WorkAreas;
	
	TMap<FGameplayTag, TArray<FTaggedWorkstationEntry>> TagToWorkstations;
	
	void RebuildWorkstationEntries(UInteractableWorkstationComponent* Workstation);
	void RemoveWorkstationEntries(UInteractableWorkstationComponent* Workstation, UPurchasableDefinition* Purchasable);
	
	bool EntryExists(UInteractableWorkstationComponent* Workstation, UPurchasableDefinition* Purchasable) const;
	void AddEntry(UInteractableWorkstationComponent* Workstation, UPurchasableDefinition* Purchasable);
	void RemoveEntry(UInteractableWorkstationComponent* Workstation, UPurchasableDefinition* Purchasable);
	void UpdateEntry(UInteractableWorkstationComponent* Workstation, UPurchasableDefinition* Purchasable);
	
	bool ShouldIncludeTag(const FGameplayTag& Tag) const;
	
	void LogTagToWorkstationsMap() const;
};
