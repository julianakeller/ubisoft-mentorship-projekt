// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "WorkstationInteractionInterface.h"
#include "WSInteractionComponentBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "MentorshipProjekt/NPCs/FamilyMember/PlayableFamilyMemberCharacter.h"
#include "MultiStageProductionComponent.generated.h"

class AFamilyMemberCharacter;

USTRUCT(BlueprintType)
struct FWorkstationStage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StageName; // e.g. "Sow", "Grow", "Harvest"
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action")
	EActionType ProductionAction; //Enum of the animation to be played during this stage

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DurationInGameMinutes = 0.f; // 0 = instant worker action, >0 = automatic timer

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsePurchasableProductionTime = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRequiresWorkerAction = true; // false = progresses automatically after duration
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPurchasableProducedAtThisStage = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseIngredientsAtThisStage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProducedAmount = 1;
	
	// Visuals
	
	// Tag for visuals for this stage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visual")
	FName VisualStageTag;
	
	// Tag for components under the visual stage component whose mesh should be replaced with the purchasable's mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visual")
	FName VisualPlaceholderTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visual")
	FName TimelineComponentName;
};

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UMultiStageProductionComponent : public UWSInteractionComponentBase
{
	GENERATED_BODY()
	
public:
	
	UMultiStageProductionComponent();

	// Configure stages per workstation type in Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stages")
	TArray<FWorkstationStage> Stages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stages")
	FName VisualStagesTag = "VisualStage";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stages")
	int32 CurrentStageIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stages")
	bool bIsProcessingAutomatically = false; 
	
	UFUNCTION(BlueprintCallable, Category="Visual")
	void OnStageChanged(int32 StageIndex);

	UFUNCTION(BlueprintCallable, Category="Visual")
	void OnStageProgress(float NormalizedProgress);
	
	UFUNCTION(BlueprintCallable, Category="Visual")
	void OnActivePurchasableChanged(UPurchasableDefinition* Purchasable);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnStageChanged(int32 StageIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnStageProgress(float NormalizedProgress);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnPurchasableChanged(UPurchasableDefinition* Purchasable);
	
	void NotifyVisualStageChanged();
	void NotifyVisualProgress(float Alpha);
	void NotifyPurchasableChanged();
	
	// Called by BT task to start interaction
	virtual bool StartWork(APlayableFamilyMemberCharacter* Worker) override;

	// Called by BT task AbortTask or when shift ends
	virtual void StopWork() override;

	// BrainComponent uses this to decide if workstation needs a worker
	virtual bool NeedsAttention(APlayableFamilyMemberCharacter* AttentionFromCharacter) override;
	virtual bool NeedsAttention() override;
	
	virtual bool SetNextProductionSettings() override;
	
	virtual void BeginPlay() override;
	
protected:
	
	virtual void UpdateProgressTick() override;
	
private:
	
	FTimerHandle AutomaticStageTimerHandle;
	FTimerHandle WorkerActionTimerHandle;
	FTimerHandle ProgressUpdateTimerHandle;

	void StartAutomaticStage();
	void OnAutomaticStageComplete();

	void StartWorkerActionStage();
	void OnWorkerActionComplete();

	void AdvanceToNextStage();
	void CompleteCurrentStage();
	
	UTimelineComponent* FindTimelineByName(FName Name) const;
	
	UPROPERTY()
	UTimelineComponent* CurrentTimeline;
	
	void PlayAnimationTimeline(const FWorkstationStage& Stage);
	void ResetTimeline(const FWorkstationStage& Stage);
	
	void SetVisualStageVisibility(const FWorkstationStage& Stage);
	
	
};