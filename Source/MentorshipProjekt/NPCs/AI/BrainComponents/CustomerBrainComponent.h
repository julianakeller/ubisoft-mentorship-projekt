// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBrainComponent.h"
#include "Components/ActorComponent.h"
#include "MentorshipProjekt/Areas/WorkAreaManager.h"
#include "MentorshipProjekt/InteractionSystem/InteractableComponents/TendingWorkstationComponent.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerInstanceData.h"
#include "MentorshipProjekt/NPCs/Customer/DesiredTagEntry.h"
#include "MentorshipProjekt/Purchasables/PurchasableDefinition.h"
#include "CustomerBrainComponent.generated.h"


struct FCustomerInstanceData;
class ACustomerCharacter;

USTRUCT()
struct FCustomerDecision
{
	GENERATED_BODY()

	UPROPERTY()
	bool bHasValidPurchase = false;
	
	UPROPERTY()
	bool bReservedWorkstation = false;

	UPROPERTY()
	AInteractableBase* TargetWorkstation = nullptr;

	UPROPERTY()
	UPurchasableDefinition* DesiredItem = nullptr;
	
	UPROPERTY()
	int32 DesiredQuanity = 1;
	
	UPROPERTY()
	TArray<FGameplayTag> MatchedTags;

	UPROPERTY()
	FVector TargetInteractionPoint;
	
	UPROPERTY()
	bool bShouldLeave = false;
	
	UPROPERTY()
	FVector LeaveLocation;
};

USTRUCT()
struct FPurchaseCandidateInformation
{
	GENERATED_BODY()

	int32 Score = 0;

	UPROPERTY()
	TArray<FGameplayTag> MatchedTags;
	
	UPROPERTY()
	int32 TargetQuantity = 1;

	UPROPERTY()
	TArray<TObjectPtr<UInteractableWorkstationComponent>> Workstations;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCustomerDecisionUpdated, const FCustomerDecision&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MENTORSHIPPROJEKT_API UCustomerBrainComponent : public UNPCBrainComponent
{
	GENERATED_BODY()

public:	
	UCustomerBrainComponent();
	
	void Initialize(ACustomerCharacter* InOwner);

	// Called by AIController to trigger purchase evaluation
	FCustomerDecision EvaluatePurchase();
	
	void EvaluateStay();

	// Called by TendingWorkstationComponent when transaction is complete
	void OnOrderServed();

	FOnCustomerDecisionUpdated OnDecisionUpdated;

	FCustomerDecision& GetCurrentDecision() { return CurrentDecision; }

	// How long customer will wait before leaving (in real seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customer")
	float PatienceSeconds = 30.f;
	
	UPROPERTY()
	bool bReservedWorkstation = false;
	
	virtual AInteractableBase* GetTargetWorkstation() override;

protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY()
	UWorkAreaManager* WorkAreaManager = nullptr;

	FCustomerDecision CurrentDecision;
	
	int32 CalculateScore(const UPurchasableDefinition* Item, const FDesiredTagEntry& Desired);

	AInteractableBase* SelectBestWorkstation(TArray<TObjectPtr<UInteractableWorkstationComponent>> Workstations);
	
	FCustomerInstanceData* GetInstanceData();
	
	TMap<FGameplayTag, FDesiredTagEntry>* GetShoppingList();
};
