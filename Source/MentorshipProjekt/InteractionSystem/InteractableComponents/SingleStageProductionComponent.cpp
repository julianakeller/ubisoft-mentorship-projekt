// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleStageProductionComponent.h"
#include "MentorshipProjekt/UI/CircularProgressWidget.h"
#include "MentorshipProjekt/NPCs/FamilyMember/PlayableFamilyMemberCharacter.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"
#include "InteractableWorkstationComponent.h"
#include "FamilyMemberReservationComponent.h"
#include "MentorshipProjekt/GameTime/GameTimeSubsystem.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyInstanceData.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyMemberCharacter.h"
#include "MentorshipProjekt/Player/TopDownCharacter.h"
#include "MentorshipProjekt/Purchasables/PurchasableManagerSubsystem.h"
#include "MentorshipProjekt/Purchasables/RecipeDefinition.h"

USingleStageProductionComponent::USingleStageProductionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USingleStageProductionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void USingleStageProductionComponent::ProduceNextItem()
{
	if (!bWorkerIsActing || !ProductionWorkstation || !GameTimeSubsystem || !PurchasableManager)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerProductionComponent: Production not possible."));
		return;
	}
	
	if (!SetNextProductionSettings())
	{
		return;
	}
	
	bool bSuccess = true;
	// Check inventory for required ingredients and remove from inventory:
	if (PurchasableManager && Recipe)
	{
		bSuccess = PurchasableManager->RemoveIngredientsIfAvailable(Recipe->Ingredients);
	}
		
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerProductionComponent: Not enough ingredients."));
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Blue, "Not enough ingredients!");
		StopWork();
		return;
	}
	
	// Convert recipe production time to real world seconds:
	ActionStartTime = GetWorld()->GetTimeSeconds();
	ActionDuration = Recipe ? GameTimeSubsystem->ToSeconds(Recipe->BaseProductionTimeInMinutes) : 5.f; // ToDo
	
	GetWorld()->GetTimerManager().SetTimer(ProductionTimerHandle, this, &USingleStageProductionComponent::HandleProductionComplete, ActionDuration, false);
	
	GetWorld()->GetTimerManager().SetTimer(ProductionUpdateTimerHandle, this, &USingleStageProductionComponent::UpdateProgressTick, 0.05f, true);
}

void USingleStageProductionComponent::HandleProductionComplete()
{
	GetWorld()->GetTimerManager().ClearTimer(ProductionUpdateTimerHandle);

	if (ProducingCharacter)
	{
		UCircularProgressWidget* Widget = ProducingCharacter->GetProductionProgressWidget();
		if (Widget)
		{
			Widget->SetProgress(1.f);
		}
	}

	if (!PurchasableManager)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerProductionComponent: No PurchasableManager found. Please fix :)"))
		return;
	}
	
	UPurchasableDefinition* Purchasable = CurrentSetting->Purchasable;
	
	if (Purchasable && Recipe)
	{
		if (PurchasableManager->ProducePurchasable(Purchasable, GetProducerSkillLevel()))
		{
			FWorkstationProductionSetting ModifiedSetting = *CurrentSetting;
			ModifiedSetting.TargetAmount--;
			ProductionWorkstation->SetProductionSetting(ModifiedSetting);
			
			// Notify listeners that production settings changed
			OnWorkCompleteDelegate.Broadcast(ProducingCharacter);
    
			// Notify workstation so BrainComponent can re-evaluate
			if (ProductionWorkstation)
			{
				ProductionWorkstation->NotifyProductionSettingsChanged();
			}
		}
	}
	
	//Todo cancel condition, e.g. exiting range
	
	ProduceNextItem();
}

bool USingleStageProductionComponent::StartWork(APlayableFamilyMemberCharacter* Worker)
{
	if (!Super::StartWork(Worker))
	{
		return false;
	}
	
	// Show production widget
	SetCharacterActionTag(ProductionActionTag);
	ShowProgressWidget();
	
	ProduceNextItem();
	
	return true;
}

void USingleStageProductionComponent::StopWork()
{
	Super::StopWork();
}

bool USingleStageProductionComponent::NeedsAttention()
{
	if (!Super::NeedsAttention())
	{
		return false;
	}

	const FWorkstationProductionSetting* Setting = ProductionWorkstation->GetHighestPriorityEnabledSetting();
	return Setting != nullptr && Setting->TargetAmount > 0;
}