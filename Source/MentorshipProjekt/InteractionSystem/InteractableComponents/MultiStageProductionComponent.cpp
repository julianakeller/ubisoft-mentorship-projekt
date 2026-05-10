// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiStageProductionComponent.h"
#include "MentorshipProjekt/GameTime/GameTimeSubsystem.h"
#include "MentorshipProjekt/Purchasables/PurchasableManagerSubsystem.h"
#include "MentorshipProjekt/Purchasables/RecipeDefinition.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyMemberCharacter.h"
#include "MentorshipProjekt/NPCs/AI/BrainComponents/FamilyMemberBrainComponent.h"
#include "MentorshipProjekt/NPCs/FamilyMember/PlayableFamilyMemberCharacter.h"
#include "Shifts/ShiftManager.h"

void UMultiStageProductionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (Stages.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("MultiStageProductionComponent: No stages configured on %s"),*GetOwner()->GetName());
	}
	else
	{
		if (Stages.IsValidIndex(CurrentStageIndex))
		{
			const FWorkstationStage& Stage = Stages[CurrentStageIndex];
			SetVisualStageVisibility(Stage);
		}
	}
}

UMultiStageProductionComponent::UMultiStageProductionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

#pragma region IWorkstationInteractionInterface

void UMultiStageProductionComponent::UpdateProgressTick()
{
	Super::UpdateProgressTick();
	
	float Elapsed = GetWorld()->GetTimeSeconds() - ActionStartTime;
	float Progress = FMath::Clamp(Elapsed / ActionDuration, 0.f, 1.f);
	
	NotifyVisualProgress(Progress);
}

bool UMultiStageProductionComponent::StartWork(APlayableFamilyMemberCharacter* Worker)
{
	if (!Super::StartWork(Worker))
	{
		return false;
	}
	
	if (!Worker || !Stages.IsValidIndex(CurrentStageIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("MultiStageProductionComponent: No worker or valid stage index found on %s"),*GetOwner()->GetName());
		return false;
	}

	const FWorkstationStage& Stage = Stages[CurrentStageIndex];

	if (CurrentStageIndex == 0)
	{
		if (!SetNextProductionSettings())
		{
			return false;
		}
	}
	
	if (Stage.bRequiresWorkerAction) //only start worker action if current stage requires worker action
	{
		if (!Recipe || (Recipe && GetProducerSkillLevel() >= Recipe->RequiredSkillLevel))
		{
			UE_LOG(LogTemp, Display, TEXT("MultiStageProductionComponent: Starting worker action production at  %s"),*GetOwner()->GetName());
			StartWorkerActionStage();
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MultiStageProductionComponent: cannot start worker action production at %s. Recipe is null."),*GetOwner()->GetName());
			StopWork();
			return false;
		}
	}
	return true;
}

void UMultiStageProductionComponent::StopWork()
{
	GetWorld()->GetTimerManager().ClearTimer(WorkerActionTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ProgressUpdateTimerHandle);
	//don't clear automatic stage timer so that e.g. fields keep growing

	Super::StopWork();
}

bool UMultiStageProductionComponent::NeedsAttention(APlayableFamilyMemberCharacter* AttentionFromCharacter)
{
	if (!Super::NeedsAttention(AttentionFromCharacter))
	{
		return false;
	}
	
	return NeedsAttention();
}

bool UMultiStageProductionComponent::NeedsAttention()
{
	if (!Super::NeedsAttention())
	{
		return false;
	}
	
	if (Stages.Num() == 0)
	{
		return false;
	}
	
	if (!Stages.IsValidIndex(CurrentStageIndex))
	{
		return false;
	}

	const FWorkstationStage& Stage = Stages[CurrentStageIndex];
	
	if (Stage.bUseIngredientsAtThisStage)
	{
		if (PurchasableManager)
		{
			URecipeDefinition* RecipeDef = ProductionWorkstation->GetHighestPriorityEnabledSetting()->Purchasable->Recipe;

			if (RecipeDef && !PurchasableManager->IngredientsAvailable(RecipeDef->Ingredients))
			{
				return false;
			}
		}
	}

	// Needs attention if current stage requires a worker and isn't being processed automatically
	return Stage.bRequiresWorkerAction && !bIsProcessingAutomatically;
}

bool UMultiStageProductionComponent::SetNextProductionSettings()
{
	bool success = Super::SetNextProductionSettings();
	
	if (success)
	{
		NotifyPurchasableChanged();
	}
	return success;
}

#pragma endregion 

#pragma region Stage Progression

void UMultiStageProductionComponent::StartWorkerActionStage()
{
	if (!Stages.IsValidIndex(CurrentStageIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("MultiStageProductionComponent: not a valid stage index on %s"),*GetOwner()->GetName());
		return;
	}

	const FWorkstationStage& Stage = Stages[CurrentStageIndex];
	
	//Remove ingredients from inventory if needed:
	if (Stage.bUseIngredientsAtThisStage && Recipe)
	{
		bool bSuccess = PurchasableManager->RemoveIngredientsIfAvailable(Recipe->Ingredients);
		
		if (!bSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("MultiStageProductionComponent: Required ingredients not available for %s"),*GetOwner()->GetName());
			StopWork();
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("MultiStageProductionComponent: Used ingredients for production at %s"),*GetOwner()->GetName());
		}
	}
	
	bWorkerIsActing = true;
	SetCharacterActionTag(Stage.ProductionAction);

	ShowProgressWidget();
	SetVisualStageVisibility(Stage);
	PlayAnimationTimeline(Stage);
	
	float Duration = Stage.bUsePurchasableProductionTime && Recipe ? Recipe->BaseProductionTimeInMinutes : Stage.DurationInGameMinutes;

	if (Duration > 0.f && GameTimeSubsystem)
	{
		// Worker action takes time — show progress and wait
		ActionStartTime = GetWorld()->GetTimeSeconds();
		ActionDuration = GameTimeSubsystem->ToSeconds(Duration);

		GetWorld()->GetTimerManager().SetTimer(
			WorkerActionTimerHandle,
			this,
			&UMultiStageProductionComponent::OnWorkerActionComplete,
			ActionDuration,
			false);

		GetWorld()->GetTimerManager().SetTimer(
			ProgressUpdateTimerHandle,
			this,
			&UMultiStageProductionComponent::UpdateProgressTick,
			0.05f,
			true);
	}
	else
	{
		// Instant worker action
		OnWorkerActionComplete();
	}
}

void UMultiStageProductionComponent::OnWorkerActionComplete()
{
	UE_LOG(LogTemp, Display, TEXT("MultiStageProductionComponent: Completed worker action at %s"),*GetOwner()->GetName());
	
	GetWorld()->GetTimerManager().ClearTimer(WorkerActionTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ProgressUpdateTimerHandle);

	bWorkerIsActing = false;
	ClearCharacterActionTag();

	UpdateProgressWidget(1.f);
	HideProgressWidget();

	//OnWorkCompleteDelegate.Broadcast(ProducingCharacter);
	
	CompleteCurrentStage();
}

void UMultiStageProductionComponent::CompleteCurrentStage()
{
	if (!Stages.IsValidIndex(CurrentStageIndex))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("MultiStageProductionComponent: completed stage at %s"),*GetOwner()->GetName());
	
	const FWorkstationStage& Stage = Stages[CurrentStageIndex];
	
	//If purchasable producing stage add produced purchasable to inventory:
	if (Stage.bPurchasableProducedAtThisStage)
	{
		UPurchasableDefinition* Purchasable = CurrentSetting->Purchasable;
		
		if (Purchasable && PurchasableManager->ProducePurchasable(Purchasable, GetProducerSkillLevel(), Stage.ProducedAmount))
		{
			FWorkstationProductionSetting ModifiedSetting = *CurrentSetting;
			ModifiedSetting.TargetAmount = FMath::Max(0, ModifiedSetting.TargetAmount - 1);
			ProductionWorkstation->SetProductionSetting(ModifiedSetting);
		}
	}

	// Advance to next stage (wraps around so field resets to Unsown after Harvest)
	AdvanceToNextStage();
}

UTimelineComponent* UMultiStageProductionComponent::FindTimelineByName(FName Name) const
{
	if (CurrentTimeline)
	{
		return CurrentTimeline;
	}
	
	TArray<UTimelineComponent*> Timelines;
	GetOwner()->GetComponents<UTimelineComponent>(Timelines);

	for (UTimelineComponent* Timeline : Timelines)
	{
		if (Timeline && Timeline->GetFName() == Name)
		{
			return Timeline;
		}
	}

	return nullptr;
}

void UMultiStageProductionComponent::PlayAnimationTimeline(const FWorkstationStage& Stage)
{
	if (!Stage.TimelineComponentName.IsNone())
	{
		UTimelineComponent* Timeline = FindTimelineByName(Stage.TimelineComponentName);

		if (Timeline)
		{
			Timeline->Stop();
			Timeline->SetPlaybackPosition(0.f, false);
			Timeline->PlayFromStart();
		}
	}
}

void UMultiStageProductionComponent::AdvanceToNextStage()
{
	CurrentStageIndex = (CurrentStageIndex + 1) % Stages.Num();

	NotifyVisualStageChanged();
	
	if (ProductionWorkstation)
	{
		ProductionWorkstation->NotifyProductionSettingsChanged();
	}
	
	const FWorkstationStage& NextStage = Stages[CurrentStageIndex];
	
	// Set next production setting if next stage is first stage
	if (CurrentStageIndex == 0)
	{
		if (!SetNextProductionSettings())
		{
			UE_LOG(LogTemp, Warning, TEXT("MultiStageProductionComponent: couldn't set next production setting on %s"),*GetOwner()->GetName());
			
			StopWork();
			
			return;
		}
	}

	if (!NextStage.bRequiresWorkerAction)
	{
		UE_LOG(LogTemp, Display, TEXT("MultiStageProductionComponent: starting automatic stage at %s"),*GetOwner()->GetName());
		// Next stage is automatic, start it immediately
		StartAutomaticStage();
	}
	else
	{
		// Next stage needs a worker, broadcast so BrainComponent re-evaluates
		OnWorkCompleteDelegate.Broadcast(ProducingCharacter);
	}
}

void UMultiStageProductionComponent::StartAutomaticStage()
{
	if (!Stages.IsValidIndex(CurrentStageIndex))
	{
		return;
	}

	const FWorkstationStage& Stage = Stages[CurrentStageIndex];

	if (!GameTimeSubsystem || Stage.DurationInGameMinutes <= 0.f)
	{
		// No duration, advance immediately
		OnAutomaticStageComplete();
		return;
	}

	bIsProcessingAutomatically = true;

	float Seconds = GameTimeSubsystem->ToSeconds(Stage.DurationInGameMinutes);
	GetWorld()->GetTimerManager().SetTimer(AutomaticStageTimerHandle,this, &UMultiStageProductionComponent::OnAutomaticStageComplete, Seconds, false);
	
	SetVisualStageVisibility(Stage);
	// Start Animation:
	PlayAnimationTimeline(Stage);
}

void UMultiStageProductionComponent::OnAutomaticStageComplete()
{
	bIsProcessingAutomatically = false;
	
	UE_LOG(LogTemp, Display, TEXT("MultiStageProductionComponent: automatic stage completed at %s"),*GetOwner()->GetName());
	
	CompleteCurrentStage();
}

void UMultiStageProductionComponent::OnStageChanged(int32 StageIndex)
{
	CurrentTimeline = nullptr;
	
	if (!Stages.IsValidIndex(StageIndex))
	{
		return;
	}
	const FWorkstationStage& Stage = Stages[StageIndex];

	// Reset timeline if defined
	ResetTimeline(Stage);

	BP_OnStageChanged(StageIndex);

}

void UMultiStageProductionComponent::SetVisualStageVisibility(const FWorkstationStage& Stage)
{
	if (!GetOwner() || !CurrentSetting || !CurrentSetting->Purchasable)
	{
		return;
	}

	TArray<USceneComponent*> SceneComponents;
	GetOwner()->GetComponents<USceneComponent>(SceneComponents);

	const TArray<UStaticMesh*>& Meshes = CurrentSetting->Purchasable->WorldMeshes;
	const int32 MeshCount = Meshes.Num();

	for (USceneComponent* Comp : SceneComponents)
	{
		// Any comp with VisualPlaceholderTag should get the purchassable static mesh
		if (Comp->ComponentHasTag(Stage.VisualPlaceholderTag))
		{
			UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Comp);
			if (!MeshComp)
			{
				continue;
			}
			
			if (MeshCount > 0)
			{
				const int32 Index = FMath::RandRange(0, MeshCount - 1);
				MeshComp->SetStaticMesh(Meshes[Index]);
			}

			if (CurrentSetting->Purchasable->WorldMaterial)
			{
				MeshComp->SetMaterial(0, CurrentSetting->Purchasable->WorldMaterial);
			}
		}
		
		// Only make comps with VisualStagesTag invisibile
		if (!Comp || !Comp->ComponentHasTag(VisualStagesTag))
		{
			continue;
		}

		const bool bVisible = Comp->ComponentHasTag(Stage.VisualStageTag);
		Comp->SetVisibility(bVisible, true);
	}
}

void UMultiStageProductionComponent::ResetTimeline(const FWorkstationStage& Stage)
{
	if (!Stage.TimelineComponentName.IsNone())
	{
		UTimelineComponent* Timeline = FindTimelineByName(Stage.TimelineComponentName);

		if (Timeline)
		{
			Timeline->Stop();
			Timeline->SetPlaybackPosition(0.f, false);
		}
	}
}

void UMultiStageProductionComponent::OnStageProgress(float NormalizedProgress)
{
	/* OLD timeline scrubbing
	if (!Stages.IsValidIndex(CurrentStageIndex))
	{
		return;
	}

	const FWorkstationStage& Stage = Stages[CurrentStageIndex];

	if (!Stage.TimelineComponentName.IsNone())
	{
		UTimelineComponent* Timeline = FindTimelineByName(Stage.TimelineComponentName);

		if (Timeline)
		{
			float Length = Timeline->GetTimelineLength();
			Timeline->SetPlaybackPosition(NormalizedProgress * Length, false);
		}
	}
	*/

	BP_OnStageProgress(NormalizedProgress);
}

void UMultiStageProductionComponent::OnActivePurchasableChanged(UPurchasableDefinition* Purchasable)
{
	if (!Purchasable)
	{
		return;
	}
	if (!Stages.IsValidIndex(CurrentStageIndex))
	{
		return;
	}

	TArray<UStaticMeshComponent*> MeshComponents;
	GetOwner()->GetComponents<UStaticMeshComponent>(MeshComponents);

	for (UStaticMeshComponent* Comp : MeshComponents)
	{
		if (!Comp) continue;

		// Use tag to identify placeholders
		if (Comp->ComponentHasTag(Stages[CurrentStageIndex].VisualPlaceholderTag))
		{
			Comp->SetStaticMesh(Purchasable->WorldMeshes[FMath::RandRange(0, Purchasable->WorldMeshes.Num() - 1)]);

			if (Purchasable->WorldMaterial)
			{
				Comp->SetMaterial(0, Purchasable->WorldMaterial);
			}
		}
	}

	BP_OnPurchasableChanged(Purchasable);
}

void UMultiStageProductionComponent::NotifyVisualStageChanged()
{
	OnStageChanged(CurrentStageIndex);
}

void UMultiStageProductionComponent::NotifyVisualProgress(float Alpha)
{
	OnStageProgress(Alpha);
}

void UMultiStageProductionComponent::NotifyPurchasableChanged()
{
	
}

#pragma endregion 