// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProductionComponent.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"
#include "InteractableWorkstationComponent.h"
#include "TopDownCharacter.h"
#include "MentorshipProjekt/Public/GameTimeSubsystem.h"
#include "MentorshipProjekt/Purchasables/RecipeDefinition.h"

UPlayerProductionComponent::UPlayerProductionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerProductionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GameTimeSubsystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>();
	PurchasableManager = GetWorld()->GetSubsystem<UPurchasableManagerSubsystem>();
	if (CachedInteractable)
	{
		ProductionWorkstation = CachedInteractable->FindComponentByClass<UInteractableWorkstationComponent>();
	}

	if (!ProductionWorkstation)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerProductionComponent: PlayerProductionComponent requires a InteractableWorkstationComponent on the Interactable."));
	}
}

void UPlayerProductionComponent::OnInteractSecondary(AActor* Interactor)
{
	if (!ProducingPlayer)
	{
		ProducingPlayer = Interactor;
	}
	if (!bIsProducing)
	{
		StartProduction();
	}
	else
	{
		StopProduction();
	}
}

void UPlayerProductionComponent::StopProduction()
{
	bIsProducing = false;
	
	// ToDo stop timer
	// ToDo stop animation
	// ToDo hide production widget
	
	ProducingPlayer = nullptr;
}

void UPlayerProductionComponent::StartProduction()
{
	if (!ProducingPlayer)
	{
		return;
	}
	
	bIsProducing = true;
	
	ProduceNextItem();
}

void UPlayerProductionComponent::ProduceNextItem()
{
	if (!bIsProducing || !ProductionWorkstation || !GameTimeSubsystem || !PurchasableManager)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerProductionComponent: Production not possible."));
		return;
	}
		
	CurrentSetting = ProductionWorkstation->GetHighestPriorityEnabledSetting();
	if (!CurrentSetting)
	{
		StopProduction();
		return;
	}
	
	UPurchasableDefinition* Purchasable = CurrentSetting->Purchasable;
	if (!Purchasable || !Purchasable->Recipe)
	{
		StopProduction();
		return;
	}
	
	Recipe = Purchasable->Recipe;
	
	// ToDo check inventory for required ingredients and remove from inventory
	
	if (ProducingPlayer)
	{
		// ToDo play animation
	}
	
	// Convert recipe production time to real world seconds:
	float ProductionTimeSeconds = GameTimeSubsystem->ToSeconds(Recipe->BaseProductionTimeInMinutes);
	
	GetWorld()->GetTimerManager().SetTimer(ProductionTimerHandle, this, &UPlayerProductionComponent::OnProductionComplete, ProductionTimeSeconds, false);
}

void UPlayerProductionComponent::OnProductionComplete()
{
	if (!PurchasableManager)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerProductionComponent: No PurchasableManager found. Please fix :)"))
		return;
	}
	
	UPurchasableDefinition* Purchasable = CurrentSetting->Purchasable;
	
	if (Purchasable && Recipe)
	{
		int32 PlayerSkillLevel = 1;
		ATopDownCharacter* PlayerCharacter = Cast<ATopDownCharacter>(ProducingPlayer);
		if (PlayerCharacter)
		{
			FSkillProgress* PlayerSkillProgress = PlayerCharacter->AcquiredSkills.Find(Recipe->RequiredSkill);
			if (PlayerSkillProgress)
			{
				PlayerSkillLevel = PlayerSkillProgress->CurrentLevel;
			}
		}
		
		PurchasableManager->ProducePurchasable(Purchasable, PlayerSkillLevel);
		
		CurrentSetting->TargetAmount--;
	}
	
	ProduceNextItem();
}