// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionWidget.h"

#include "InteractionRowWidget.h"

void UInteractionWidget::PopulateRows(TArray<FInteractionEntry> Rows, bool bIsInteracting)
{
	if (!InteractionWidgetRowContainer)
	{
		return;
	}

	InteractionWidgetRowContainer->ClearChildren();

	if (!InteractionRowWidgetClass.IsValid())
	{
		InteractionRowWidgetClass.LoadSynchronous();
	}

	UClass* RowClass = InteractionRowWidgetClass.Get();

	if (!RowClass)
	{
		return;
	}

	for (const FInteractionEntry& Entry : Rows)
	{
		UInteractionRowWidget* RowWidget = CreateWidget<UInteractionRowWidget>(GetWorld(), RowClass);

		if (!RowWidget)
		{
			continue;
		}

		RowWidget->SetupRow(Entry, bIsInteracting);

		InteractionWidgetRowContainer->AddChildToVerticalBox(RowWidget);
	}
}
