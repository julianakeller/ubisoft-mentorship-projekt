// Fill out your copyright notice in the Description page of Project Settings.


#include "WSInteractionComponentBase.h"

#include "MentorshipProjekt/NPCs/FamilyMember/FamilyMemberCharacter.h"
#include "MentorshipProjekt/NPCs/Skills/SkillProgress.h"
#include "MentorshipProjekt/Player/TopDownCharacter.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyInstanceData.h"
#include "MentorshipProjekt/Purchasables/RecipeDefinition.h"
#include "MentorshipProjekt/UI/CircularProgressWidget.h"

int32 UWSInteractionComponentBase::GetPriority() const
{
	if (!ProductionWorkstation)
	{
		return -1;
	}

	const FWorkstationProductionSetting* Setting = ProductionWorkstation->GetHighestPriorityEnabledSetting();
	if (!Setting)
	{
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Blue, "Nothing to produce at this workstation.");
	}
	return Setting ? Setting->Priority : -1;
}

bool UWSInteractionComponentBase::UseTaskTimeout() const
{
	return true;
}

bool UWSInteractionComponentBase::NeedsAttention(APlayableFamilyMemberCharacter* AttentionFromCharacter)
{
	if (bWorkerIsActing && ProducingCharacter != AttentionFromCharacter)
	{
		return false;
	}
	else
	{
		return NeedsAttention();
	}
}

bool UWSInteractionComponentBase::NeedsAttention()
{
	if (!ProductionWorkstation)
	{
		return false;
	}
	
	if (!ProductionWorkstation->GetHighestPriorityEnabledSetting())
	{
		return false;
	}
	
	return true;
}

bool UWSInteractionComponentBase::StartWork(APlayableFamilyMemberCharacter* Worker)
{
	if (!Worker)
	{
		return false;
	}
	
	if (FamilyMemberReservationComponent)
	{
		if (!FamilyMemberReservationComponent->TryReserve(Worker))
		{
			UE_LOG(LogTemp, Log, TEXT("WSInteractionComponentBase: Already reserved by another worker."));
			StopWork();
			return false;
		}
	}
	
	ProducingCharacter = Worker;
	bWorkerIsActing = true;
	return true;
}

void UWSInteractionComponentBase::StopWork()
{
	HideProgressWidget();
	
	bWorkerIsActing = false;
	ClearCharacterActionTag();

	if (FamilyMemberReservationComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("MultiStageProductionComponent: clearing reservation on %s"),*GetOwner()->GetName());
		FamilyMemberReservationComponent->ClearReservation();
	}

	OnWorkStoppedDelegate.Broadcast(ProducingCharacter);

	ProducingCharacter = nullptr;
}

int32 UWSInteractionComponentBase::GetProducerSkillLevel()
{
	int32 SkillLevel = 1;
		
	if (ProducingCharacter)
	{
		FSkillProgress* CharacterSkillProgress = nullptr;
		if (Recipe)
		{
			if (ATopDownCharacter* PlayerCharacter = Cast<ATopDownCharacter>(ProducingCharacter))
			{
				CharacterSkillProgress = PlayerCharacter->AcquiredSkills.Find(Recipe->RequiredSkill);
			}
			else if (AFamilyMemberCharacter* FamilyMemberCharacter = Cast<AFamilyMemberCharacter>(ProducingCharacter))
			{
				CharacterSkillProgress = FamilyMemberCharacter->GetInstanceData().AcquiredSkills.Find(Recipe->RequiredSkill);
			}
			if (CharacterSkillProgress)
			{
				SkillLevel = CharacterSkillProgress->CurrentLevel;
			}
		}
	}
	
	UE_LOG(LogTemp, Display, TEXT("WSInteractionComponentBase: Producer skill level for workstation %s is %d"),*GetOwner()->GetName(), SkillLevel);
	
	return SkillLevel;
}

bool UWSInteractionComponentBase::SetNextProductionSettings()
{
	CurrentSetting = ProductionWorkstation->GetHighestPriorityEnabledSetting();
	if (!CurrentSetting)
	{
		UE_LOG(LogTemp, Warning, TEXT("WSInteractionComponentBase: no current setting found on %s"),*GetOwner()->GetName());
		StopWork();
		return false;
	}
	UPurchasableDefinition* Purchasable = CurrentSetting->Purchasable;
	if (!Purchasable)
	{
		UE_LOG(LogTemp, Warning, TEXT("WSInteractionComponentBase: no purchasable found for current setting on %s"),*GetOwner()->GetName());
		StopWork();
		return false;
	}
	Recipe = Purchasable->Recipe;
	return true;
}

void UWSInteractionComponentBase::ShowProgressWidget()
{
	if (!ProducingCharacter) return;
	UCircularProgressWidget* Widget = ProducingCharacter->GetProductionProgressWidget();
	if (Widget) Widget->EnableWidget();
}

void UWSInteractionComponentBase::HideProgressWidget()
{
	if (!ProducingCharacter) return;
	UCircularProgressWidget* Widget = ProducingCharacter->GetProductionProgressWidget();
	if (Widget) Widget->DisableWidget();
}

void UWSInteractionComponentBase::UpdateProgressWidget(float Progress)
{
	if (!ProducingCharacter) return;
	UCircularProgressWidget* Widget = ProducingCharacter->GetProductionProgressWidget();
	if (Widget) Widget->SetProgress(Progress);
}

void UWSInteractionComponentBase::UpdateProgressTick()
{
	float Elapsed = GetWorld()->GetTimeSeconds() - ActionStartTime;
	float Progress = FMath::Clamp(Elapsed / ActionDuration, 0.f, 1.f);
	UpdateProgressWidget(Progress);
}

float UWSInteractionComponentBase::UpdateProgress()
{
	float Elapsed = GetWorld()->GetTimeSeconds() - ActionStartTime;
	float Progress = FMath::Clamp(Elapsed / ActionDuration, 0.f, 1.f);
	UpdateProgressWidget(Progress);
	return Progress;
}

void UWSInteractionComponentBase::SetCharacterActionTag(EActionType Tag)
{
	if (!ProducingCharacter) return;
	UE_LOG(LogTemp, Display, TEXT("WSInteractionComponentBase: Action tag for workstation %s set to %s"),*GetOwner()->GetName(), *StaticEnum<EActionType>()->GetValueAsString(Tag));
	ProducingCharacter->CurrentAction = Tag;
}

void UWSInteractionComponentBase::ClearCharacterActionTag()
{
	if (!ProducingCharacter) return;
	ProducingCharacter->CurrentAction = EActionType::None;
}
