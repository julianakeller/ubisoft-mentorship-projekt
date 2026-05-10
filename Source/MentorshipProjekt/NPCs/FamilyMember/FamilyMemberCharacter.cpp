// Fill out your copyright notice in the Description page of Project Settings.


#include "FamilyMemberCharacter.h"

#include "../AI/AIController/FamilyMemberAIController.h"
#include "../AI/BrainComponents/FamilyMemberBrainComponent.h"
#include "Components/WidgetComponent.h"
#include "MentorshipProjekt/NPCs/Simulation/FamilySimulationSubsystem.h"

AFamilyMemberCharacter::AFamilyMemberCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	AIControllerClass = AFamilyMemberAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	BrainComponent = CreateDefaultSubobject<UFamilyMemberBrainComponent>(TEXT("FamilyBrainComponent"));
	
	NameLabelWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameLabelWidgetComponent"));
	NameLabelWidgetComponent->SetupAttachment(GetMesh());
	NameLabelWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	NameLabelWidgetComponent->SetDrawSize(FVector2D(150.f, 40.f));
	NameLabelWidgetComponent->SetVisibility(true);
}

void AFamilyMemberCharacter::Initialize(FFamilyInstanceData* InData, FGuid InFamilyInstanceId)
{
	FamilyMemberId = InFamilyInstanceId;
	
	if (InData && InData->FamilyMemberData)
	{
		CachedName = InData->FamilyMemberData->FamilyMemberName;
	}
	
}

void AFamilyMemberCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
	
	if (UUserWidget* Widget = NameLabelWidgetComponent->GetUserWidgetObject())
	{
		NameLabelWidget = Cast<UFamilyMemberNameLabelWidget>(Widget);

		if (NameLabelWidget)
		{
			NameLabelWidget->SetNameText(FText::FromName(CachedName));
		}
	}
}

void AFamilyMemberCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FFamilyInstanceData AFamilyMemberCharacter::GetInstanceData() const
{
	UFamilySimulationSubsystem* Subsystem = GetWorld()->GetSubsystem<UFamilySimulationSubsystem>();
	if (!Subsystem)
	{
		return FFamilyInstanceData();
	}

	const FFamilyInstanceData* Data = Subsystem->FamilyMembers.Find(FamilyMemberId);
	if (!Data)
	{
		return FFamilyInstanceData();
	}

	return *Data; // returns a copy
}

