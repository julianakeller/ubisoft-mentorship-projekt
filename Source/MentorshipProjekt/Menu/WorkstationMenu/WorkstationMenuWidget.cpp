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
		WorkstationComponent = Source->FindComponentByClass<UInteractableWorkstationComponent>();	
	}
}

void UWorkstationMenuWidget::Open()
{
	Super::Open();

	if (!WorkstationComponent)
		return;
	
	WorkstationComponent->RebuildSortedIndices();

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
		WorkstationComponent->RebuildSortedIndices();
	}

	Super::Close();
}

void UWorkstationMenuWidget::PopulateRows()
{
	if (!VerticalBox || !RowWidgetClass)
		return;

	VerticalBox->ClearChildren();

	for (int32 i = 0; i < WorkstationComponent->SortedSettingIndices.Num(); ++i)
	{
		const FWorkstationProductionSetting* Setting = WorkstationComponent->GetSettingSorted(i);
		if (!Setting || !Setting->Purchasable) continue;

		UWorkstationMenuRowWidget* Row = CreateWidget<UWorkstationMenuRowWidget>(GetWorld(), RowWidgetClass);
		if (!Row) continue;

		Row->InitializeRow(WorkstationComponent, Setting->Purchasable, WorkstationComponent->SortedSettingIndices[i]);

		VerticalBox->AddChild(Row);
	}
}