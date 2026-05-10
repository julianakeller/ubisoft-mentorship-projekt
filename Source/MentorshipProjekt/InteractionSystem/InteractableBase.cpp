// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "MentorshipProjekt/Player/TopDownCharacter.h"

AInteractableBase::AInteractableBase()
{
 	PrimaryActorTick.bCanEverTick = false;
	
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableMesh"));
	RootComponent = InteractableMesh;
	
	// Create Interaction Range Sphere:
	InteractionRange = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRange"));
	InteractionRange->InitSphereRadius(200.f);
	InteractionRange->SetupAttachment(InteractableMesh);
	
	// Create Interaction Widget Compmonent subobject:
	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractionWidgetComponent->SetupAttachment(InteractableMesh);
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidgetComponent->SetDrawAtDesiredSize(true);
	InteractionWidgetComponent->SetHiddenInGame(true, true);
}

void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionRange->OnComponentBeginOverlap.AddDynamic(this, &AInteractableBase::OnBeginOverlap);
	InteractionRange->OnComponentEndOverlap.AddDynamic(this, &AInteractableBase::OnEndOverlap);
}

void AInteractableBase::SetInteractionState(EInteractionState NewState)
{
	if (InteractionState == NewState)
	{
		return;
	}

	InteractionState = NewState;
}

void AInteractableBase::OnInteractionRangeEntered()
{
	if (InteractionState == EInteractionState::Idle)
	{
		SetInteractionState(EInteractionState::Available);
	}

	// Forward to components
	OnRangeEnteredEvent.Broadcast(nullptr);
}

void AInteractableBase::OnInteractionRangeExited()
{
	if (InteractionState != EInteractionState::Interacting)
	{
		SetInteractionState(EInteractionState::Idle);
	}

	// Forward to components
	OnRangeExitedEvent.Broadcast(nullptr);
}

void AInteractableBase::OnInteracted(AActor* Character)
{
	if (!CanBeInteracted())
	{
		return;
	}
	
	HandleInteraction(Character);
}

void AInteractableBase::OnInteractedSecondary(AActor* Character)
{
	if (!CanBeInteracted())
	{
		return;
	}
	
	HandleInteractionSecondary(Character);
}

void AInteractableBase::HandleInteraction(AActor* Character)
{
	//Debug Message:
	//GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Green, "Interacted!");
	
	// Execute Interactions in Components:
	OnInteractEvent.Broadcast(Character);
}

void AInteractableBase::HandleInteractionSecondary(AActor* Character)
{
	//GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Orange, "Interacted!");
	
	OnInteractSecondaryEvent.Broadcast(Character);
}

bool AInteractableBase::CanBeInteracted() const
{
	return InteractionState == EInteractionState::Available
		|| InteractionState == EInteractionState::Interacting;
}

void AInteractableBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}
	
	ATopDownCharacter* PlayerCharacter = Cast<ATopDownCharacter>(OtherActor);

	if (PlayerCharacter)
	{
		bPlayerInRange = true;
	}
	
	OnInteractionRangeEntered();
}

void AInteractableBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}
	
	ATopDownCharacter* PlayerCharacter = Cast<ATopDownCharacter>(OtherActor);

	if (PlayerCharacter)
	{
		bPlayerInRange = false;
	}

	OnInteractionRangeExited();
}