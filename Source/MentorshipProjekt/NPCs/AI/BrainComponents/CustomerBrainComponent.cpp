// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerBrainComponent.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"
#include "MentorshipProjekt/InteractionSystem/InteractableComponents/CustomerReservationComponent.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerCharacter.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerInstanceData.h"
#include "MentorshipProjekt/NPCs/Simulation/CustomerSimulationSubsystem.h"
#include "MentorshipProjekt/NPCs/SpawnPoints/CustomerSpawnPoint.h"

// Sets default values for this component's properties
UCustomerBrainComponent::UCustomerBrainComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCustomerBrainComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCustomerBrainComponent::Initialize(ACustomerCharacter* InOwner)
{
	Super::Initialize(InOwner);
}

FCustomerDecision UCustomerBrainComponent::EvaluatePurchase()
{
	UE_LOG(LogTemp, Warning, TEXT("UCustomerBrainComponent::EvaluatePurchase | START"));

	CurrentDecision = FCustomerDecision();
	
	if (!WorkAreaManager)
	{
		WorkAreaManager = GetWorld()->GetSubsystem<UWorkAreaManager>();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("UCustomerBrainComponent::EvaluatePurchase | OwnerCharacter=%s WorkAreaManager=%s"),
	OwnerCharacter ? TEXT("VALID") : TEXT("NULL"), WorkAreaManager ? TEXT("VALID") : TEXT("NULL"));
	
	if (!OwnerCharacter || !WorkAreaManager)
	{
		OnDecisionUpdated.Broadcast(CurrentDecision);
		UE_LOG(LogTemp, Warning, TEXT("UCustomerBrainComponent::EvaluatePurchase | FAILED early: missing OwnerCharacter or WorkAreaManager"));
		return CurrentDecision;
	}
	
	const TMap<FGameplayTag, FDesiredTagEntry>* ShoppingList = GetShoppingList();
	
	UE_LOG(LogTemp, Warning, TEXT("UCustomerBrainComponent::EvaluatePurchase | ShoppingList ptr=%s"), ShoppingList ? TEXT("VALID") : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("UCustomerBrainComponent::EvaluatePurchase | ShoppingList size=%d"),
	ShoppingList ? ShoppingList->Num() : -1);
	
	if (!ShoppingList || ShoppingList->Num() == 0)
	{
		OnDecisionUpdated.Broadcast(CurrentDecision);
		UE_LOG(LogTemp, Warning, TEXT("UCustomerBrainComponent::EvaluatePurchase | FAILED early: empty or invalid ShoppingList"));
		return CurrentDecision;
	}
	
	bool bHasDemand = false;
	
	for (const TPair<FGameplayTag, FDesiredTagEntry>& Pair : *ShoppingList)
	{
		const FDesiredTagEntry& Entry = Pair.Value;

		if (Entry.QuantityRemaining > 0)
		{
			bHasDemand = true;
			break;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("UCustomerBrainComponent::EvaluatePurchase | Demand found=%s"), bHasDemand ? TEXT("true") : TEXT("false"));

	if (!bHasDemand)
	{
		OnDecisionUpdated.Broadcast(CurrentDecision);
		UE_LOG(LogTemp, Warning, TEXT("UCustomerBrainComponent::EvaluatePurchase | FAILED early: no demand"));
		return CurrentDecision;
	}
	
	TMap<UPurchasableDefinition*, FPurchaseCandidateInformation> CandidateMap;

	// Build aggregated scores:
	
	for (const TPair<FGameplayTag, FDesiredTagEntry>& Pair : *ShoppingList)
	{
		const FDesiredTagEntry& Desired = Pair.Value;
		
		if (Desired.QuantityRemaining <= 0)
		{
			continue;
		}

		const TArray<FTaggedWorkstationEntry>* Entries = WorkAreaManager->GetEntriesForTag(Desired.Tag);

		if (!Entries)
		{
			UE_LOG(LogTemp, Warning, TEXT("UCustomerBrainComponent::EvaluatePurchase | No entries for Tag %s"), *Desired.Tag.ToString());
			continue;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UCustomerBrainComponent::EvaluatePurchase | Found workstation entries for Tag %s"), *Desired.Tag.ToString());
		}

		for (const FTaggedWorkstationEntry& Entry : *Entries)
		{
			if (!Entry.Purchasable || !Entry.Workstation)
			{
				continue;
			}
			
			UPurchasableManagerSubsystem* PurchasableManager = GetWorld()->GetSubsystem<UPurchasableManagerSubsystem>();
			
			if (PurchasableManager)
			{
				const int32 Available = PurchasableManager->GetPurchasableCountByDefinition(Entry.Purchasable);
				if (Available <= 0) continue;
			}
			
			// Get or create candidate
			FPurchaseCandidateInformation& Candidate = CandidateMap.FindOrAdd(Entry.Purchasable);

			// Score contribution
			int32 Contribution = CalculateScore(Entry.Purchasable, Desired);

			Candidate.Score += Contribution;

			Candidate.MatchedTags.AddUnique(Desired.Tag);
			Candidate.Workstations.Add(Entry.Workstation);
			
			Candidate.TargetQuantity = Desired.QuantityRemaining;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("UCustomerBrainComponent::EvaluatePurchase | CandidateMap build COMPLETE | size=%d"),
	CandidateMap.Num());
	
	// Select best purchasable for purchase:
	
	UPurchasableDefinition* BestItem = nullptr;
	FPurchaseCandidateInformation* BestCandidate = nullptr;
	int32 BestScore = 0;

	for (TPair<UPurchasableDefinition*, FPurchaseCandidateInformation>& Pair : CandidateMap)
	{
		FPurchaseCandidateInformation& Candidate = Pair.Value;

		if (Candidate.Score > BestScore)
		{
			BestScore = Candidate.Score;
			BestItem = Pair.Key;
			BestCandidate = &Candidate;
		}
	}
	
	// Apply purchasable selection to decision:
	
	if (BestItem && BestCandidate && BestCandidate->Workstations.Num() > 0)
	{
		CurrentDecision.bHasValidPurchase = true;
		CurrentDecision.DesiredItem = BestItem;
		CurrentDecision.TargetWorkstation = SelectBestWorkstation(BestCandidate->Workstations);
		CurrentDecision.MatchedTags = BestCandidate->MatchedTags;
		CurrentDecision.DesiredQuanity = BestCandidate->TargetQuantity;
		
		UCustomerReservationComponent* ReservationComponent = CurrentDecision.TargetWorkstation->FindComponentByClass<UCustomerReservationComponent>();
		if (ReservationComponent && ReservationComponent->CustomerInteractionPoint)
		{
			CurrentDecision.TargetInteractionPoint = ReservationComponent->CustomerInteractionPoint->GetComponentLocation();
		}
		else
		{
			CurrentDecision.TargetInteractionPoint = CurrentDecision.TargetWorkstation->GetActorLocation();
		}
	}
	
	OnDecisionUpdated.Broadcast(CurrentDecision);
	
	return CurrentDecision;
}

void UCustomerBrainComponent::EvaluateStay()
{
	if (!OwnerCharacter)
    {
        return;
    }
	ACustomerCharacter* CustomerCharacter = Cast<ACustomerCharacter>(OwnerCharacter);
	if (!CustomerCharacter)
	{
		return;
	}

    AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController());

    if (!AIController)
    {
        return;
    }

    UBlackboardComponent* BB = AIController->GetBlackboardComponent();

    if (!BB)
    {
        return;
    }

    const TMap<FGameplayTag, FDesiredTagEntry>* ShoppingList = GetShoppingList();

    const int32 RemainingItems = ShoppingList ? ShoppingList->Num() : 0;

    const float PurchaseFactor = FMath::Clamp( CustomerCharacter->SuccessfulPurchases / 3.f, 0.f, 1.f);

    // More remaining shopping lowers leave chance
    const float ShoppingModifier = RemainingItems > 0 ? 1.f / (float)(RemainingItems + 1) : 1.f;

    const float LeaveProbability = FMath::Clamp(
            PurchaseFactor * ShoppingModifier,
            0.f,
            1.f);

    const bool bShouldLeave = FMath::FRand() <= LeaveProbability;

    BB->SetValueAsBool(TEXT("ShouldLeave"), bShouldLeave);

    UE_LOG(LogTemp, Display,
        TEXT("EvaluateStay | Purchases=%d Remaining=%d LeaveProbability=%.2f Leave=%s"),
        CustomerCharacter->SuccessfulPurchases,
        RemainingItems,
        LeaveProbability,
        bShouldLeave ? TEXT("true") : TEXT("false"));

    if (!bShouldLeave)
    {
        return;
    }

    // Find CustomerSpawnPoint
	TArray<AActor*> SpawnPoints;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomerSpawnPoint::StaticClass(),SpawnPoints);

	if (SpawnPoints.Num() == 0)
	{
		return;
	}

	const int32 Index = FMath::RandRange(0, SpawnPoints.Num() - 1);

	AActor* SelectedSpawnPoint = SpawnPoints[Index];

	if (!SelectedSpawnPoint)
	{
		return;
	}

	const FVector LeaveLocation = SelectedSpawnPoint->GetActorLocation();

    BB->SetValueAsVector(TEXT("LeaveLocation"), LeaveLocation);
}

AInteractableBase* UCustomerBrainComponent::GetTargetWorkstation()
{
	return CurrentDecision.TargetWorkstation;
}

AInteractableBase* UCustomerBrainComponent::SelectBestWorkstation(TArray<TObjectPtr<UInteractableWorkstationComponent>> Workstations)
{
	// ToDo dependant on distance and existing reservations
	return Cast<AInteractableBase>(Workstations[0]->GetOwner());
}

void UCustomerBrainComponent::OnOrderServed()
{
	
	if (ACustomerCharacter* Character = Cast<ACustomerCharacter>(OwnerCharacter))
	{
		Character->SuccessfulPurchases++;
	}
	
	TMap<FGameplayTag, FDesiredTagEntry>* ShoppingList = GetShoppingList();

	if (!ShoppingList)
	{
		return;
	}

	for (const FGameplayTag& Tag : CurrentDecision.MatchedTags)
	{
		FDesiredTagEntry* Entry = ShoppingList->Find(Tag);

		if (!Entry)
		{
			continue;
		}

		Entry->QuantityRemaining--;

		if (Entry->QuantityRemaining <= 0)
		{
			ShoppingList->Remove(Tag);
		}
	}

	CurrentDecision = FCustomerDecision();
}

int32 UCustomerBrainComponent::CalculateScore(const UPurchasableDefinition* Item, const FDesiredTagEntry& Desired)
{
	if (!Item)
	{
		return 0;
	}
	if (Desired.QuantityRemaining <= 0)
	{
		return 0;
	}

	if (!Item->ItemTags.HasTag(Desired.Tag))
	{
		return 0;	
	}

	return Desired.Priority * Desired.QuantityRemaining;
}

FCustomerInstanceData* UCustomerBrainComponent::GetInstanceData()
{
	if (OwnerCharacter)
	{
		if (ACustomerCharacter* CustomerCharacter = Cast<ACustomerCharacter>(OwnerCharacter))
		{
			return CustomerCharacter->GetInstanceDataRef();
		}
	}
	return nullptr;
}

TMap<FGameplayTag, FDesiredTagEntry>* UCustomerBrainComponent::GetShoppingList()
{
	FCustomerInstanceData* InstanceData = GetInstanceData();
	UCustomerSimulationSubsystem* CustomerSim = GetWorld()->GetSubsystem<UCustomerSimulationSubsystem>();
	if (!InstanceData || !CustomerSim)
	{
		return nullptr;
	}
	return CustomerSim->GetHouseholdShoppingList(InstanceData->HouseholdId);
}