// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProductionComponent.h"
#include "SingleStageProductionComponent.h"
#include "FamilyMemberReservationComponent.h"
#include "MentorshipProjekt/Purchasables/RecipeDefinition.h"
#include "MultiStageProductionComponent.h"
#include "MentorshipProjekt/NPCs/FamilyMember/PlayableFamilyMemberCharacter.h"
#include "MentorshipProjekt/Player/TopDownCharacter.h"

UPlayerProductionComponent::UPlayerProductionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerProductionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerProductionComponent::OnInteractSecondary(AActor* Interactor)
{
	if (ATopDownCharacter* Character = Cast<ATopDownCharacter>(Interactor))
	{
		if (ProductionComponent && !ProductionComponent->bWorkerIsActing)
		{
			UE_LOG(LogTemp, Display, TEXT("PlayerProductionComponent: Starting Production"))
			ProductionComponent->GetOnWorkComplete().AddUObject(this, &UPlayerProductionComponent::HandleProductionFinished);
			ProductionComponent->StartWork(Character);
			//Character->BeginWorkstationLock(ProductionComponent->GetOwner());
		}
		else if (ProductionComponent && ProductionComponent->ProducingCharacter == Character)
		{
			UE_LOG(LogTemp, Display, TEXT("PlayerProductionComponent: Stopping Production"))
			ProductionComponent->GetOnWorkComplete().RemoveAll(this);
			ProductionComponent->StopWork();
			Character->EndWorkstationLock();
		}
		
		if (!ProductionComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerProductionComponent: Could not find ProductionComponent"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerProductionComponent: Could not Cast to APlayableFamilyMemberCharacter"))
	}
}

void UPlayerProductionComponent::HandleProductionFinished(APlayableFamilyMemberCharacter* Interactor)
{
	if (!ProductionComponent)
	{
		return;
	}
	
	if (UMultiStageProductionComponent* MultiStageComponent = Cast<UMultiStageProductionComponent>(ProductionComponent))
	{
		if (ShouldContinueProducing())
		{
			MultiStageComponent->StartWork(Cast<APlayableFamilyMemberCharacter>(Interactor));
			return;
		}
	}
	if (ATopDownCharacter* Character = Cast<ATopDownCharacter>(Interactor))
	{
		Character->EndWorkstationLock();
	}
}

bool UPlayerProductionComponent::ShouldContinueProducing() const
{
	/*
	if (!ProductionWorkstation)
	{
		return false;
	}

	FWorkstationProductionSetting* Setting = ProductionWorkstation->GetHighestPriorityEnabledSetting();
	if (!Setting || Setting->TargetAmount <= 0)
	{
		return false;
	}

	// Check ingredients
	if (Setting->Purchasable && Setting->Purchasable->Recipe)
	{
		if (!PurchasableManager->IngredientsAvailable(Setting->Purchasable->Recipe->Ingredients))
		{
			return false;
		}
	}*/

	return false;
}