// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkstationMenuRowWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SpinBox.h"
#include "Components/Image.h"
#include "MentorshipProjekt/InteractionSystem/InteractableComponents/InteractableWorkstationComponent.h"

void UWorkstationMenuRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ButtonEnable)
	{
		ButtonEnable->OnClicked.AddDynamic(this, &UWorkstationMenuRowWidget::OnEnableButtonClicked);
	}

	if (TargetAmount)
	{
		TargetAmount->OnValueChanged.AddDynamic(this, &UWorkstationMenuRowWidget::OnAmountValueChanged);
	}
	if (TargetPriority)
	{
		TargetPriority->OnValueChanged.AddDynamic(this, &UWorkstationMenuRowWidget::OnPriorityValueChanged);
	}
}

void UWorkstationMenuRowWidget::InitializeRow(UInteractableWorkstationComponent* InComponent, UPurchasableDefinition* InPurchasable, int32 InIndex)
{
	WorkstationComponent = InComponent;
	Purchasable = InPurchasable;
	SettingIndex = InIndex;

	if (PurchasableName && Purchasable)
	{
		PurchasableName->SetText(FText::FromName(Purchasable->ItemName));
	}

	if (FWorkstationProductionSetting* Setting = WorkstationComponent->GetSetting(SettingIndex))
	{
		if (TargetAmount)
		{
			TargetAmount->SetValue(Setting->TargetAmount);
		}
		if (TargetPriority)
		{
			TargetPriority->SetValue(Setting->Priority);
		}
	}

	RefreshVisuals();
}

void UWorkstationMenuRowWidget::OnEnableButtonClicked()
{
	if (!WorkstationComponent)
		return;

	FWorkstationProductionSetting* Setting = WorkstationComponent->GetSetting(SettingIndex);

	if (!Setting)
		return;

	Setting->bEnabled = !Setting->bEnabled;

	RefreshVisuals();
}

void UWorkstationMenuRowWidget::OnAmountValueChanged(float Value)
{
	if (!WorkstationComponent)
		return;

	FWorkstationProductionSetting* Setting = WorkstationComponent->GetSetting(SettingIndex);

	if (!Setting)
		return;

	Setting->TargetAmount = FMath::RoundToInt(Value);
}

void UWorkstationMenuRowWidget::OnPriorityValueChanged(float Value)
{
	if (!WorkstationComponent)
		return;

	FWorkstationProductionSetting* Setting = WorkstationComponent->GetSetting(SettingIndex);

	if (!Setting)
		return;

	Setting->Priority = FMath::RoundToInt(Value);
}

void UWorkstationMenuRowWidget::RefreshVisuals()
{
	if (!WorkstationComponent || !Checkmark)
		return;

	const FWorkstationProductionSetting* Setting = WorkstationComponent->GetSetting(SettingIndex);

	if (!Setting)
		return;

	FSlateBrush Brush;
	if (Setting->bEnabled && CheckedImage)
	{
		Brush.SetResourceObject(CheckedImage);
	}
	else if (!Setting->bEnabled && UncheckedImage)
	{
		Brush.SetResourceObject(UncheckedImage);
	}
	else
	{
		Checkmark->SetBrush(FSlateBrush());
		return;
	}

	Checkmark->SetBrush(Brush);
}