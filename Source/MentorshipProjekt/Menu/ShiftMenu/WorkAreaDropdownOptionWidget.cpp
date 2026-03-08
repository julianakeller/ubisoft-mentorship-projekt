// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkAreaDropdownOptionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UWorkAreaDropdownOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &UWorkAreaDropdownOptionWidget::HandleClicked);
	}
}

//Setup Dropdown Option according to work area it represents
void UWorkAreaDropdownOptionWidget::InitializeOption(FName InWorkAreaID, const FText& InDisplayName)
{
	WorkAreaID = InWorkAreaID;

	if (OptionText)
	{
		OptionText->SetText(InDisplayName);
	}
}

void UWorkAreaDropdownOptionWidget::HandleClicked()
{
	OnSelected.Broadcast(WorkAreaID);
}