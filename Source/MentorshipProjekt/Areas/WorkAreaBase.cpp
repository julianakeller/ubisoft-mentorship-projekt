// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkAreaBase.h"
#include "VisitAreaComponent.h"
#include "WorkAreaManager.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "MentorshipProjekt/InteractionSystem/InteractableComponents/TendingWorkstationComponent.h"
#include "MentorshipProjekt/UI/AreaSignWidget.h"

AWorkAreaBase::AWorkAreaBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	WorkAreaExtents = CreateDefaultSubobject<UBoxComponent>(TEXT("WorkAreaExtents"));
	WorkAreaExtents->SetupAttachment(Root);
	WorkAreaExtents->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WorkAreaExtents->SetCollisionResponseToAllChannels(ECR_Overlap);
	WorkAreaExtents->SetLineThickness(2.f);

	WorkAreaLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("WorkAreaLabel"));
	WorkAreaLabel->SetupAttachment(WorkAreaExtents);
	WorkAreaLabel->SetHorizontalAlignment(EHTA_Center);
	WorkAreaLabel->SetVerticalAlignment(EVRTA_TextBottom);
	WorkAreaLabel->SetWorldSize(50.f);
	WorkAreaLabel->bHiddenInGame = true;
}

void AWorkAreaBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (WorkAreaExtents)
	{
		WorkAreaExtents->ShapeColor = WorkAreaColor.ToFColor(true);
	}

	if (WorkAreaLabel)
	{
		WorkAreaLabel->SetText(FText::FromName(WorkAreaName));
		WorkAreaLabel->SetTextRenderColor(WorkAreaColor.ToFColor(true));

		// label position above extends box
		FVector BoxExtent = WorkAreaExtents ? WorkAreaExtents->GetScaledBoxExtent() : FVector::ZeroVector;
		WorkAreaLabel->SetRelativeLocation(FVector(0.f, 0.f, BoxExtent.Z + 20.f));
	}
}

void AWorkAreaBase::BeginPlay()
{
	Super::BeginPlay();
	
	//Defer to ensure workstations are initialized before gathering them
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &AWorkAreaBase::GatherWorkStations, 0.5f, false);
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
	WorkStations.Empty();
	CustomerWorkstations.Empty();
	
	if (!WorkAreaExtents)
	{
		return;
	}

	TArray<AActor*> OverlappingActors;
	WorkAreaExtents->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor)
		{
			continue;
		}
		
		if (AInteractableBase* Interactable = Cast<AInteractableBase>(Actor))
		{
			if (Interactable->FindComponentByClass<UTendingWorkstationComponent>())
			{
				CustomerWorkstations.Add(Interactable);
			}
			WorkStations.Add(Interactable);
			UE_LOG(LogTemp, Log, TEXT("GetOverlappingActors: WorkStation added: %s"), *Interactable->GetName());
		}
		
		// Find widget components to set area sign name:
		TArray<UWidgetComponent*> WidgetComponents;
		Actor->GetComponents<UWidgetComponent>(WidgetComponents);
		
		UE_LOG(LogTemp,Log,TEXT("GetOverlappingActors: Actor '%s' has %d widget components"),*Actor->GetName(),WidgetComponents.Num());

		for (UWidgetComponent* WidgetComp : WidgetComponents)
		{
			if (!WidgetComp)
			{
				continue;
			}
			
			UE_LOG(LogTemp,Log,TEXT("GetOverlappingActors: Checking WidgetComponent '%s'"),*WidgetComp->GetName());

			UAreaSignWidget* AreaSignWidget = Cast<UAreaSignWidget>(WidgetComp->GetUserWidgetObject());

			if (!AreaSignWidget)
			{
				continue;
			}
			
			UE_LOG(LogTemp,Log,TEXT("GetOverlappingActors: Setting area name '%s' on AreaSignWidget"),*WorkAreaName.ToString());

			AreaSignWidget->SetAreaName(WorkAreaName);
		}
	}
	
	//Register workarea with WorkAreaManager
	if (UWorkAreaManager* Subsystem = GetWorld()->GetSubsystem<UWorkAreaManager>())
	{
		Subsystem->RegisterWorkArea(this);
	}
}
