// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskInteractWithWorkstation.h"

#include "MentorshipProjekt/InteractionSystem/InteractableComponents/SingleStageProductionComponent.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyMemberCharacter.h"
#include "MentorshipProjekt/NPCs/FamilyMember/PlayableFamilyMemberCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MentorshipProjekt/InteractionSystem/InteractableComponents/InteractableWorkstationComponent.h"
#include "MentorshipProjekt/Purchasables/RecipeDefinition.h"

class UTendingWorkstationComponent;

UBTTaskInteractWithWorkstation::UBTTaskInteractWithWorkstation()
{
	NodeName = "Interact With Workstation";
	
	bCreateNodeInstance = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTaskInteractWithWorkstation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bTaskFinished = false;
	ClearCachedState();
	
	UE_LOG(LogTemp, Warning, TEXT("InteractWithWorkstation: Executing InteractWithWorkstationTask"));
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractWithWorkstation: No controller found"));
		return EBTNodeResult::Failed;
	}

	AFamilyMemberCharacter* Character = Cast<AFamilyMemberCharacter>(Controller->GetPawn());
	if (!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractWithWorkstation: No Character found"));
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractWithWorkstation: No Blackboard found"));
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetWorkstation"));
	if (!Target)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractWithWorkstation: No TargetWorkstation found"));
		return EBTNodeResult::Failed;
	}
	CachedWorkstationComponent = Target->FindComponentByClass<UInteractableWorkstationComponent>();
	if (!CachedWorkstationComponent)
	{
		return EBTNodeResult::Failed;
	}
	
	// Find interaction interface:
	for (UActorComponent* Comp : Target->GetComponents())
	{
		CachedProductionInterface = Cast<IWorkstationInteractionInterface>(Comp);
		if (CachedProductionInterface)
		{
			break;
		}
	}

	if (!CachedProductionInterface)
	{
		return EBTNodeResult::Failed;
	}
	
	// Check if there's anything left to produce/tend to before starting
	if (!CachedProductionInterface->NeedsAttention())
	{
		return EBTNodeResult::Failed;
	}

	// Bind to completion/production-stop delegate instead of returning immediately
	CachedProductionInterface->GetOnWorkComplete().AddUObject(this, &UBTTaskInteractWithWorkstation::OnProductionCycleComplete);
	CachedProductionInterface->GetOnWorkStopped().AddUObject(this, &UBTTaskInteractWithWorkstation::OnProductionStopped);
	UInteractableWorkstationComponent* WSC = Target->FindComponentByClass<UInteractableWorkstationComponent>();
	if (WSC)
	{
		WSC->OnProductionSettingsChanged.AddUObject(this, &UBTTaskInteractWithWorkstation::OnProductionSettingsChanged);
		
		// disable timeout if Tending workstation component
		bUseTimeout = CachedProductionInterface->UseTaskTimeout();
		if (bUseTimeout)
		{
			if (const FWorkstationProductionSetting* Setting = WSC->GetHighestPriorityEnabledSetting())
			{
				if (UPurchasableDefinition* Purchasable = Setting->Purchasable)
				{
					if (URecipeDefinition* Recipe = Purchasable->Recipe)
					{
						if (UGameTimeSubsystem* TimeSubsystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>())
						{
							// Set task timeout to production duration
							MaxTaskDuration = 2.f * TimeSubsystem->ToSeconds(Recipe->BaseProductionTimeInMinutes);
						}
					}
				}
			}
		}
	}
	
	CachedOwnerComp = &OwnerComp;

	CachedProductionInterface->StartWork(Character);
	
	StartTimeout(OwnerComp);

	return EBTNodeResult::InProgress;
}

void UBTTaskInteractWithWorkstation::OnProductionCycleComplete(APlayableFamilyMemberCharacter* Interactor)
{
	if (!CachedOwnerComp || !CachedProductionInterface)
	{
		return;
	}

	// Check if more items still need to be produced/tended to
	if (!CachedProductionInterface->NeedsAttention())
	{
		// No worker needed anymore -> leave task immediately
		FinishTaskSafe(EBTNodeResult::Succeeded);
	}
	// Continue working (e.g. next worker stage)
}

void UBTTaskInteractWithWorkstation::OnProductionStopped(APlayableFamilyMemberCharacter* Interactor)
{
	FinishTaskSafe(EBTNodeResult::Failed);
}

void UBTTaskInteractWithWorkstation::OnProductionSettingsChanged(UInteractableWorkstationComponent* WSC)
{
	FinishTaskSafe(EBTNodeResult::Succeeded);
}

void UBTTaskInteractWithWorkstation::ClearCachedState()
{
	CleanupTask();
	CachedProductionInterface = nullptr;
	CachedWorkstationComponent = nullptr;
}

void UBTTaskInteractWithWorkstation::CleanupTask()
{
	RemoveDelegateBindings();
	
	if (CachedProductionInterface)
	{
		CachedProductionInterface->StopWork();
	}
	
	CachedProductionInterface = nullptr;
	CachedWorkstationComponent = nullptr;
}

void UBTTaskInteractWithWorkstation::RemoveDelegateBindings() const
{
	if (CachedProductionInterface)
	{
		CachedProductionInterface->GetOnWorkComplete().RemoveAll(this);
		CachedProductionInterface->GetOnWorkStopped().RemoveAll(this);
	}
	if (CachedWorkstationComponent)
	{
		CachedWorkstationComponent->OnProductionSettingsChanged.RemoveAll(this);
	}
}
