// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableWorkstationComponent.h"
#include "WorkstationInteractionInterface.h"
#include "WSInteractionComponentBase.h"
#include "TendingWorkstationComponent.generated.h"

class ACustomerCharacter;
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTransactionComplete, bool /*bSuccess*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UTendingWorkstationComponent : public UWSInteractionComponentBase
{
	GENERATED_BODY()
	
public:
	
	UTendingWorkstationComponent();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tending")
	EActionType TendingActionTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tending")
	EActionType ServingCustomerActionTag;
	
	virtual bool StartWork(APlayableFamilyMemberCharacter* Worker) override;
	virtual void StopWork() override;
	virtual bool NeedsAttention() override;
	virtual bool NeedsAttention(APlayableFamilyMemberCharacter* AttentionFromCharacter) override;
	virtual int32 GetPriority() const override;
	
	bool UseTaskTimeout() const override;
	
	// Called by customer to initiate a purchase
	bool TryPurchaseCustomer(ACustomerCharacter* Customer, UPurchasableDefinition* RequestedItem, int32 Amount);

	FOnTransactionComplete OnTransactionComplete;
	
private:
	
	FTimerHandle OrderTimerHandle;
	FTimerHandle ProgressUpdateTimerHandle;

	// Pending transaction data set when customer places order
	UPROPERTY()
	ACustomerCharacter* PendingCustomer = nullptr;

	UPROPERTY()
	UPurchasableDefinition* PendingItem = nullptr;
	
	int32 PurchaseAmount = 1;

	//void OnCustomerArrived(ANPCCharacter* Customer);
	void OnCustomerLeft();
	bool StartServingCustomer(UPurchasableDefinition* RequestedItem, int32 Amount);
	void OnOrderComplete();

	void SubscribeToReservationEvents();
};
