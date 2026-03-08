// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkAreaDropdownWidget.h"

#include "ShiftBlockWidget.h"
#include "WorkAreaDropdownOptionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "MentorshipProjekt/Areas/WorkAreaBase.h"
#include "MentorshipProjekt/Areas/WorkAreaManager.h"
#include "Shifts/ShiftManager.h"

void UWorkAreaDropdownWidget::InitializeDropdown(UShiftBlockWidget* InOwner)
{
	OwnerShift = InOwner;
	
	if (!OptionWidgetClass || !OptionsBox) return;
	
	OptionsBox->ClearChildren();
	
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	//ShiftManager = World->GetSubsystem<UShiftManager>();
	
	const UWorkAreaManager* Manager = World->GetSubsystem<UWorkAreaManager>();
	if (!Manager)
	{
		return;
	}
	
	for (const AWorkAreaBase* Area : Manager->GetAllWorkAreas())
	{
		UWorkAreaDropdownOptionWidget* Option = CreateWidget<UWorkAreaDropdownOptionWidget>(GetWorld(), OptionWidgetClass);

		FName ID = Area->WorkAreaName;
		
		Option->InitializeOption(ID, FText::FromName(ID));
		Option->OnSelected.AddDynamic(this, &UWorkAreaDropdownWidget::HandleOptionSelected);

		OptionsBox->AddChild(Option);
	}
}

void UWorkAreaDropdownWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (OptionsBox)
	{
		OptionsBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	/*
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	const UWorkAreaManager* Manager = World->GetSubsystem<UWorkAreaManager>();
	if (!Manager)
	{
		return;
	}
	
	for (const AWorkAreaBase* Area : Manager->GetAllWorkAreas())
	{
		if (!Area)
		{
			continue;
		}
		
		UButton* Button = NewObject<UButton>(this);
		UTextBlock* TextBlock = NewObject<UTextBlock>(this);
	}
	*/
}

void UWorkAreaDropdownWidget::ToggleDropdown()
{
	bIsOpen = !bIsOpen;

	OptionsBox->SetVisibility(bIsOpen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UWorkAreaDropdownWidget::HandleOptionSelected(FName SelectedID)
{
	bIsOpen = false;
	OptionsBox->SetVisibility(ESlateVisibility::Collapsed);

	// Pass on selection to shift block widget for update of ShiftData & visual changes to shift block
	OnWorkAreaChosen.Broadcast(SelectedID);
}