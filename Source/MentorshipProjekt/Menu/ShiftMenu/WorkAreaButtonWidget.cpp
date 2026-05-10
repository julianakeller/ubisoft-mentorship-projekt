// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkAreaButtonWidget.h"

void UWorkAreaButtonWidget::InitializeButton(FName InAreaName, const FLinearColor& InColor)
{
	AreaName = InAreaName;
	AreaColor = InColor;
	
	if (ButtonText)
	{
		ButtonText->SetText(FText::FromName(AreaName));
	}
	SetSelected(false);
	UpdateVisual();
}

void UWorkAreaButtonWidget::SetSelected(bool bSelected)
{
	if (bSelected)
	{
		SelectionBorder->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SelectionBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}

FName UWorkAreaButtonWidget::GetAreaName()
{
	return AreaName;
}

void UWorkAreaButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UWorkAreaButtonWidget::HandleClicked);
	}
}

void UWorkAreaButtonWidget::HandleClicked()
{
	SetSelected(true);
	OnClicked.Broadcast(AreaName);
}

void UWorkAreaButtonWidget::UpdateVisual()
{
	if (BackgroundBorder)
	{
		BackgroundBorder->SetBrushColor(AreaColor);
	}
}
