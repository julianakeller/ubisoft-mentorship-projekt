// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "InteractionWidget.h"
#include "TopDownCharacter.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	PrimaryActorTick.bCanEverTick = false;
	
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableMesh"));
	RootComponent = InteractableMesh;
	
	InteractionRange = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRange"));
	InteractionRange->InitSphereRadius(200.f);
	InteractionRange->SetupAttachment(InteractableMesh);
	
	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractionWidgetComponent->SetupAttachment(InteractableMesh);
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidgetComponent->SetDrawAtDesiredSize(true);
	InteractionWidgetComponent->SetHiddenInGame(true, true);
	
	static ConstructorHelpers::FClassFinder<UUserWidget> InteractionWidgetClassFinder(TEXT("/Game/UI/BP_InteractionWidget"));
	if (InteractionWidgetClassFinder.Succeeded())
	{
		InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClassFinder.Class);
	}
	
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionRange->OnComponentBeginOverlap.AddDynamic(this, &AInteractableBase::OnBeginOverlap);
	InteractionRange->OnComponentEndOverlap.AddDynamic(this, &AInteractableBase::OnEndOverlap);
	
	if (InteractionWidgetComponent)
	{
		InteractionWidgetReference = Cast<UInteractionWidget>(InteractionWidgetComponent->GetWidget());
	}
}

void AInteractableBase::UpdateWidgetVisibility()
{
	//Should only be visible if player, not NPC, is near
	
	if (!InteractionWidgetComponent)
	{
		return;
	}

	const bool bShouldBeVisible = (InteractionState == EInteractionState::Available);
	
	//const bool bShouldBeVisible = bPlayerInRange && CanBeInteracted(); //OLD
	InteractionWidgetComponent->SetHiddenInGame(!bShouldBeVisible, true);
}

void AInteractableBase::SetInteractionState(EInteractionState NewState)
{
	if (InteractionState == NewState)
	{
		return;
	}

	InteractionState = NewState;
	UpdateWidgetVisibility();
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

void AInteractableBase::HandleInteraction(AActor* Character)
{
	//Debug Message:
	GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Green, "Interacted!");
	
	// Execute Interactions in Components:
	OnInteractEvent.Broadcast(Character);
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
	/*if (!Cast<ATopDownCharacter>(OtherActor)) //Only player can interact
	{
		return;
	}*/
	
	OnInteractionRangeEntered();
}

void AInteractableBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	OnInteractionRangeExited();
}

void AInteractableBase::SetInteractionText(FText NewInteractionText)
{
	InteractionText = NewInteractionText;
	if (InteractionWidgetReference)
	{
		InteractionWidgetReference->UpdateInteractionText(NewInteractionText);
	}
}