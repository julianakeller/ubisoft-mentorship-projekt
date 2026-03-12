// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkstationMenuWidget.h"

#include "WorkstationMenuRowWidget.h"
#include "Components/TextBlock.h"
#include "MentorshipProjekt/InteractionSystem/InteractableComponents/InteractableWorkstationComponent.h"

void UWorkstationMenuWidget::InitializeMenu(AInteractableBase* Source)
{
	Super::InitializeMenu(Source);
	
	if (Source)
	{
		WorkstationComponent = SourceInteractable->FindComponentByClass<UInteractableWorkstationComponent>();	
	}
}

void UWorkstationMenuWidget::Open()
{
	Super::Open();

	if (!WorkstationComponent)
		return;

	if (MenuHeader)
	{
		MenuHeader->SetText(FText::FromName(WorkstationComponent->WorkstationName));
	}

	PopulateRows();
}

void UWorkstationMenuWidget::Close()
{
	// Sort production settings before closing
	if (WorkstationComponent)
	{
		WorkstationComponent->SortProductionSettings();
	}

	Super::Close();
}

void UWorkstationMenuWidget::PopulateRows()
{
	if (!VerticalBox || !RowWidgetClass)
		return;

	VerticalBox->ClearChildren();

	for (int32 i = 0; i < WorkstationComponent->ProductionSettings.Num(); ++i)
	{
		FWorkstationProductionSetting& Setting = WorkstationComponent->ProductionSettings[i];
		if (!Setting.Purchasable) continue;

		UWorkstationMenuRowWidget* Row = CreateWidget<UWorkstationMenuRowWidget>(GetWorld(), RowWidgetClass);
		if (!Row) continue;

		Row->InitializeRow(WorkstationComponent, Setting.Purchasable, i);

		VerticalBox->AddChild(Row);
	}
}