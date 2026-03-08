// Fill out your copyright notice in the Description page of Project Settings.


#include "VisitAreaComponent.h"
#include "WorkAreaBase.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"

UVisitAreaComponent::UVisitAreaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

TArray<AInteractableBase*> UVisitAreaComponent::GetFreeInteractables() const
{
	return TArray<AInteractableBase*>(); //ToDo
}

void UVisitAreaComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GatherVisitorInteractables();
}

void UVisitAreaComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UVisitAreaComponent::GatherVisitorInteractables()
{
	// Get VisitorInteractablesRoot from the owner (WorkAreaBase)
	AWorkAreaBase* WorkArea = Cast<AWorkAreaBase>(GetOwner());
	if (!WorkArea || !WorkArea->VisitorInteractablesRoot)
	{
		UE_LOG(LogTemp, Warning, TEXT("VisitAreaComponent: No VisitorInteractablesRoot found in owner"));
		return;
	}
	VisitorInteractablesRoot = WorkArea->VisitorInteractablesRoot;
	
	if (!VisitorInteractablesRoot)
	{
		return;
	}

	VisitorInteractables.Empty();

	TArray<USceneComponent*> VisitorInteractablesChildren;
	VisitorInteractablesRoot->GetChildrenComponents(false, VisitorInteractablesChildren);

	for (USceneComponent* ChildComp : VisitorInteractablesChildren)
	{
		AActor* ChildCompOwner = ChildComp->GetOwner();
		if (AInteractableBase* Interactable = Cast<AInteractableBase>(ChildCompOwner))
		{
			VisitorInteractables.Add(Interactable);
			UE_LOG(LogTemp, Log, TEXT("Visitor Interactable added: %s"), *Interactable->GetName());
		}
	}
}