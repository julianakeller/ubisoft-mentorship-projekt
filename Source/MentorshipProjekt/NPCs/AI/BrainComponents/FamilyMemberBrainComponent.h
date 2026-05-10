// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBrainComponent.h"
#include "Components/ActorComponent.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyMemberCharacter.h"
#include "FamilyMemberBrainComponent.generated.h"


class IWorkstationInteractionInterface;
class UInteractableWorkstationComponent;
class UShiftManager;
class UGameTimeSubsystem;
struct FShiftData;
class AInteractableBase;
class UWorkAreaManager;
class AWorkAreaBase;

USTRUCT()
struct FWorkDecision
{
	GENERATED_BODY()

	UPROPERTY()
	bool bHasValidTask = false;
	
	UPROPERTY()
	bool bHasActiveShift = false;
	
	UPROPERTY()
	FName AssignedArea;

	UPROPERTY()
	AWorkAreaBase* WorkArea = nullptr;

	UPROPERTY()
	AInteractableBase* TargetWorkstation = nullptr;
	
	FVector TargetInteractionPoint;
	
	IWorkstationInteractionInterface* CurrentInterface = nullptr;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDecisionUpdated, const FWorkDecision&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MENTORSHIPPROJEKT_API UFamilyMemberBrainComponent : public UNPCBrainComponent
{
	GENERATED_BODY()
	
	//Evaluates the current shift
	//Finds the assigned work area
	//Chooses which workstation to go to
	//Keeps track of current decision
	//ProductionComponent takes care of what to produce, not Brain

public:	
	
	UFamilyMemberBrainComponent();
	
	void Initialize(class AFamilyMemberCharacter* InOwner);

	// Reevaluate what the family member should do
	FWorkDecision EvaluateWork();

	FWorkDecision& GetCurrentDecision() { return CurrentDecision; }
	
	virtual AInteractableBase* GetTargetWorkstation() override;

protected:
	
	virtual void BeginPlay() override;

private:
	
	UPROPERTY()
	UWorkAreaManager* WorkAreaManager;
	
	UPROPERTY()
	UGameTimeSubsystem* GameTimeSubsystem;
	
	UPROPERTY()
	UShiftManager* ShiftManager;

	FWorkDecision CurrentDecision;

	AWorkAreaBase* ResolveWorkArea(FName AreaName);

	// Find highest priority workstation with pending production tasks
	AInteractableBase* FindBestWorkstation(AWorkAreaBase* Area);
	
	IWorkstationInteractionInterface* FindWorkstationInteractionInterface(AInteractableBase* Workstation);
};
