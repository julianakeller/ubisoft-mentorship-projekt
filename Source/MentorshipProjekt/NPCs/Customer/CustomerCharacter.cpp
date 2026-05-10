// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerCharacter.h"

#include "Components/WidgetComponent.h"
#include "MentorshipProjekt/NPCs/AI/AIController/CustomerAIController.h"
#include "MentorshipProjekt/NPCs/Simulation/CustomerSimulationSubsystem.h"

ACustomerCharacter::ACustomerCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	BrainComponent = CreateDefaultSubobject<UCustomerBrainComponent>(TEXT("CustomerComponent"));
	
	AIControllerClass = ACustomerAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	CustomerInformationDisplayWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CustomerInformationDisplay"));
	CustomerInformationDisplayWidget->SetupAttachment(GetMesh());
	CustomerInformationDisplayWidget->SetWidgetSpace(EWidgetSpace::Screen);
	CustomerInformationDisplayWidget->SetDrawSize(FVector2D(150.f, 40.f));
	CustomerInformationDisplayWidget->SetVisibility(false);
}

void ACustomerCharacter::Initialize(FGuid InCustomerInstanceId)
{
	CustomerId = InCustomerInstanceId;
}

FCustomerInstanceData ACustomerCharacter::GetInstanceData() const
{
	UCustomerSimulationSubsystem* Subsystem = GetWorld()->GetSubsystem<UCustomerSimulationSubsystem>();
	if (!Subsystem)
	{
		return FCustomerInstanceData();
	}

	const FCustomerInstanceData* Data = Subsystem->Customers.Find(CustomerId);
	if (!Data)
	{
		return FCustomerInstanceData();
	}

	return *Data; // returns a copy
}

FCustomerInstanceData* ACustomerCharacter::GetInstanceDataRef()
{
	UCustomerSimulationSubsystem* Subsystem = GetWorld()->GetSubsystem<UCustomerSimulationSubsystem>();
	if (!Subsystem)
	{
		return nullptr;
	}

	FCustomerInstanceData* Data = Subsystem->Customers.Find(CustomerId);
	if (!Data)
	{
		return nullptr;
	}

	return Data;
}

void ACustomerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACustomerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



