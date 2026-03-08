// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkAreaBase.h"
#include "VisitAreaComponent.h"
#include "WorkAreaManager.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"
#include "Components/SceneComponent.h"

AWorkAreaBase::AWorkAreaBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	WorkStationsRoot = CreateDefaultSubobject<USceneComponent>("WorkStationsRoot");
	WorkStationsRoot->SetupAttachment(Root);
	
	//ToDo: Only create if needed (visitorAreaComponent attached)
	VisitorInteractablesRoot = CreateDefaultSubobject<USceneComponent>("VisitorInteractablesRoot");
	VisitorInteractablesRoot ->SetupAttachment(Root);
	
	//Setup debug box:
	DebugBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("DebugBounds"));
	DebugBounds->SetupAttachment(Root);
	DebugBounds->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DebugBounds->bHiddenInGame = true;
	DebugBounds->SetVisibility(true);
}

void AWorkAreaBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GatherWorkStations();
	
	UpdateDebugBounds();
}

void AWorkAreaBase::UpdateDebugBounds() const
{
	if (!DebugBounds)
	{
		return;		
	}
	
	// Start with empty box
	FBox CombinedBox(ForceInit);

	// Include all workstations
	if (WorkStationsRoot)
	{
		for (const USceneComponent* ChildComp : WorkStationsRoot->GetAttachChildren())
		{
			if (const AActor* OwnerActor = ChildComp->GetOwner())
			{
				CombinedBox += OwnerActor->GetComponentsBoundingBox(true);
			}
		}
	}

	// Include all visitor interactables
	if (VisitorInteractablesRoot)
	{
		for (const USceneComponent* ChildComp : VisitorInteractablesRoot->GetAttachChildren())
		{
			if (const AActor* OwnerActor = ChildComp->GetOwner())
			{
				CombinedBox += OwnerActor->GetComponentsBoundingBox(true);
			}
		}
	}

	// Set box extent and location
	FVector BoxExtent = CombinedBox.GetSize() * 0.5f;
	FVector BoxCenter = CombinedBox.GetCenter();

	DebugBounds->SetWorldLocation(BoxCenter);
	DebugBounds->SetBoxExtent(BoxExtent);
}

void AWorkAreaBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Detect VisitAreaComponent added in the Blueprint
	VisitAreaComponent = FindComponentByClass<UVisitAreaComponent>();
	
	//Register workarea with WorkAreaManager
	if (UWorkAreaManager* Subsystem = GetWorld()->GetSubsystem<UWorkAreaManager>())
	{
		Subsystem->RegisterWorkArea(this);
	}
}

void AWorkAreaBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorkAreaManager* Subsystem = GetWorld()->GetSubsystem<UWorkAreaManager>())
	{
		Subsystem->UnregisterWorkArea(this);
	}

	Super::EndPlay(EndPlayReason);
}

void AWorkAreaBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorkAreaBase::GatherWorkStations()
{
	if (!WorkStationsRoot)
	{
		return;
	}
	WorkStations.Empty();

	TArray<USceneComponent*> WorkstationChildren;
	WorkStationsRoot->GetChildrenComponents(false, WorkstationChildren);

	for (const USceneComponent* ChildComp : WorkstationChildren)
	{
		AActor* ChildCompOwner = ChildComp->GetOwner();
		if (AInteractableBase* Interactable = Cast<AInteractableBase>(ChildCompOwner))
		{
			WorkStations.Add(Interactable);
			UE_LOG(LogTemp, Log, TEXT("WorkStation added: %s"), *Interactable->GetName());
		}
	}
}
