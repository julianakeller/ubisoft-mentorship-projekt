// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableFamilyMemberCharacter.h"
#include "MentorshipProjekt/UI/CircularProgressWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

APlayableFamilyMemberCharacter::APlayableFamilyMemberCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProgressWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ProgressWidgetComponent"));
	ProgressWidgetComponent->SetupAttachment(GetMesh());
	ProgressWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	ProgressWidgetComponent->SetDrawSize(FVector2D(50.f, 50.f));
	ProgressWidgetComponent->SetVisibility(false);
}

void APlayableFamilyMemberCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (ProgressWidgetComponent && ProgressWidgetClass)
	{
		ProgressWidgetComponent->SetWidgetClass(ProgressWidgetClass);
		ProgressWidgetComponent->InitWidget();
	}
}

void APlayableFamilyMemberCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UCircularProgressWidget* APlayableFamilyMemberCharacter::GetProductionProgressWidget()
{
	if (!ProgressWidgetComponent)
	{
		return nullptr;
	}
	return Cast<UCircularProgressWidget>(ProgressWidgetComponent->GetUserWidgetObject());
}