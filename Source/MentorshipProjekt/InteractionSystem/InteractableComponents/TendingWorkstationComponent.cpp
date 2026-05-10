// Fill out your copyright notice in the Description page of Project Settings.


#include "TendingWorkstationComponent.h"

#include "CustomerReservationComponent.h"
#include "FamilyEconomySubsystem.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerCharacter.h"
#include "MentorshipProjekt/NPCs/Simulation/CustomerSimulationSubsystem.h"

UTendingWorkstationComponent::UTendingWorkstationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTendingWorkstationComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SubscribeToReservationEvents();
}

bool UTendingWorkstationComponent::TryPurchaseCustomer(ACustomerCharacter* Customer, UPurchasableDefinition* RequestedItem, int32 Amount)
{
	if (!Customer || !RequestedItem)
	{
		return false;
	}
	
	// Store request
	PendingCustomer = Customer;
	PendingItem = RequestedItem;

	UE_LOG(LogTemp, Log, TEXT("TendingWorkstation: Purchase requested: %s"), *RequestedItem->GetName());

	// If worker already present, serve immediately
	if (bWorkerIsActing)
	{
		return StartServingCustomer(RequestedItem, Amount);
	}

	return true; // Customer will wait, pending customer makes workers prioritize workstation
}

void UTendingWorkstationComponent::OnCustomerLeft()
{
	PendingCustomer = nullptr;
	PendingItem = nullptr;
	
	SetCharacterActionTag(TendingActionTag);

	GetWorld()->GetTimerManager().ClearTimer(OrderTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ProgressUpdateTimerHandle);

	HideProgressWidget();

	// Notify BrainComponent that worker may need to move to different workstation now
	if (ProductionWorkstation)
	{
		ProductionWorkstation->NotifyProductionSettingsChanged();
	}
}

bool UTendingWorkstationComponent::StartServingCustomer(UPurchasableDefinition* RequestedItem, int32 Amount)
{
	if (!CustomerReservationComponent) return false;

	ANPCCharacter* Customer = CustomerReservationComponent->GetReserver();
	if (!Customer)
	{
		return false;
	}
	ACustomerCharacter* CustomerCharacter = Cast<ACustomerCharacter>(Customer);
	if (!CustomerCharacter)
	{
		return false;
	}
	
	PendingCustomer = CustomerCharacter;
	PendingItem = RequestedItem;
	PurchaseAmount = Amount;

	if (!PendingItem) // ToDo check if available at this workstation
	{
		UE_LOG(LogTemp, Warning, TEXT("TendingWorkstation: No pending item to serve."));
		return false;
	}
	
	SetCharacterActionTag(ServingCustomerActionTag);

	// Order processing time — use recipe time or a flat default
	float OrderDurationMinutes = 2.f * Amount;

	ActionStartTime = GetWorld()->GetTimeSeconds();
	ActionDuration = GameTimeSubsystem ? GameTimeSubsystem->ToSeconds(OrderDurationMinutes) : 5.f;

	ShowProgressWidget();

	GetWorld()->GetTimerManager().SetTimer(
		OrderTimerHandle,
		this,
		&UTendingWorkstationComponent::OnOrderComplete,
		ActionDuration,
		false);

	GetWorld()->GetTimerManager().SetTimer(
		ProgressUpdateTimerHandle,
		this,
		&UTendingWorkstationComponent::UpdateProgressTick,
		0.05f,
		true);

	UE_LOG(LogTemp, Log, TEXT("TendingWorkstation: Serving customer with %s"), *PendingItem->GetName());
	
	return true;
}

void UTendingWorkstationComponent::OnOrderComplete()
{
	bool bSuccess = false;
	
	GetWorld()->GetTimerManager().ClearTimer(OrderTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ProgressUpdateTimerHandle);

	UpdateProgressWidget(1.f);
	HideProgressWidget();

	if (!PendingCustomer || !PendingItem || !PurchasableManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("TendingWorkstation: Order complete but missing data."));
	}
	else
	{
		// Remove item from inventory
		FPurchaseResult PurchaseResult = PurchasableManager->TakePurchasables(PendingItem, PurchaseAmount);

		// Determine price:
		
		float FinalPrice = 0;
		
		for (UPurchasableInstance* Purchasable : PurchaseResult.Instances)
		{
			const float BasePrice = PendingItem->BasePrice;
			const float Quality = Purchasable->Quality;
			const float Freshness = Purchasable->Freshness;
			
			FinalPrice += BasePrice * Quality * Freshness;
		}

		// Add gold to player, subtract from customer
		UCustomerSimulationSubsystem* CustomerSim = GetWorld()->GetSubsystem<UCustomerSimulationSubsystem>();
		
		if (CustomerSim)
		{
			CustomerSim->ModifyHouseholdFunds(PendingCustomer->GetInstanceData().HouseholdId, -FinalPrice);
			CustomerSim->AddPurchasablesToHousehold(PendingCustomer->GetInstanceData().HouseholdId, PurchaseResult.Instances);
		}
		
		if (UWorld* World = GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance())
			{
				if (UFamilyEconomySubsystem* FamilyEconomy = GameInstance->GetSubsystem<UFamilyEconomySubsystem>())
				{
					FamilyEconomy->AddGold(FinalPrice);
				}
			}
		}
		
		bSuccess = true;

		// Decrement target amount on workstation setting
		if (CurrentSetting)
		{
			FWorkstationProductionSetting ModifiedSetting = *CurrentSetting;
			ModifiedSetting.TargetAmount = FMath::Max(0, ModifiedSetting.TargetAmount - PurchaseAmount);
			ProductionWorkstation->SetProductionSetting(ModifiedSetting);
		}

		UE_LOG(LogTemp, Log, TEXT("TendingWorkstation: Transaction complete. Price: %.1f"), FinalPrice);

		PendingCustomer = nullptr;
		PendingItem = nullptr;	
	}
	
	OnTransactionComplete.Broadcast(bSuccess);
}

void UTendingWorkstationComponent::SubscribeToReservationEvents()
{
	if (!CustomerReservationComponent)
	{
		return;
	}
	CustomerReservationComponent->OnReservationEnded.AddUObject(this, &UTendingWorkstationComponent::OnCustomerLeft);
}

bool UTendingWorkstationComponent::StartWork(APlayableFamilyMemberCharacter* Worker)
{
	if (!Super::StartWork(Worker))
	{
		return false;
	}
	
	SetCharacterActionTag(TendingActionTag);
	
	UE_LOG(LogTemp, Log, TEXT("TendingWorkstation: Worker started tending %s"), *GetOwner()->GetName());
	
	if (PendingCustomer && PendingItem)
	{
		UE_LOG(LogTemp, Log, TEXT("TendingWorkstation: Worker arrived, found waiting customer. Starting service."));
		StartServingCustomer(PendingItem, PurchaseAmount);
	}
	
	return true;
}

void UTendingWorkstationComponent::StopWork()
{
	GetWorld()->GetTimerManager().ClearTimer(OrderTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ProgressUpdateTimerHandle);
	
	PendingCustomer = nullptr;
	PendingItem = nullptr;
	
	Super::StopWork();
}

bool UTendingWorkstationComponent::NeedsAttention(APlayableFamilyMemberCharacter* AttentionFromCharacter)
{
	if (!Super::NeedsAttention(AttentionFromCharacter))
	{
		return false;
	}
	
	return NeedsAttention();
}

bool UTendingWorkstationComponent::NeedsAttention()
{
	if (!Super::NeedsAttention())
	{
		return false;
	}
	
	if (CustomerReservationComponent && CustomerReservationComponent->IsReserved())
	{
		return true;
	}
	
	const FWorkstationProductionSetting* Setting = ProductionWorkstation->GetHighestPriorityEnabledSetting();
	return Setting != nullptr && Setting->TargetAmount > 0;
}

int32 UTendingWorkstationComponent::GetPriority() const
{
	if (Super::GetPriority() == -1)
	{
		return -1;
	}
	
	if (CustomerReservationComponent && CustomerReservationComponent->IsReserved())
	{
		return INT_MAX;
	}

	const FWorkstationProductionSetting* Setting = ProductionWorkstation->GetHighestPriorityEnabledSetting();
	return Setting ? Setting->Priority : -1;
}

bool UTendingWorkstationComponent::UseTaskTimeout() const
{
	return false;
}
