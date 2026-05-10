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
		TargetAmount->OnValueCommitted.AddDynamic(this, &UWorkstationMenuRowWidget::OnAmountValueComitted);
	}
	if (TargetPriority)
	{
		TargetPriority->OnValueCommitted.AddDynamic(this, &UWorkstationMenuRowWidget::OnPriorityValueComitted);
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

	if (const FWorkstationProductionSetting* Setting = WorkstationComponent->GetProductionSetting(SettingIndex))
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
	{
		return;
	}

	const FWorkstationProductionSetting* Setting = WorkstationComponent->GetProductionSetting(SettingIndex);

	if (!Setting)
	{
		return;
	}

	FWorkstationProductionSetting ModifiedSetting = *Setting;
	ModifiedSetting.bEnabled = !ModifiedSetting.bEnabled;

	WorkstationComponent->SetProductionSetting(ModifiedSetting);
	
	RefreshVisuals();
}

void UWorkstationMenuRowWidget::OnAmountValueComitted(float Value, ETextCommit::Type CommitType)
{
	if (!WorkstationComponent)
		return;

	const FWorkstationProductionSetting* Setting = WorkstationComponent->GetProductionSetting(SettingIndex);

	if (!Setting)
		return;

	FWorkstationProductionSetting ModifiedSetting = *Setting;
	ModifiedSetting.TargetAmount = FMath::RoundToInt(Value);
	
	WorkstationComponent->SetProductionSetting(ModifiedSetting);
}

void UWorkstationMenuRowWidget::OnPriorityValueComitted(float Value, ETextCommit::Type CommitType)
{
	if (!WorkstationComponent)
		return;

	const FWorkstationProductionSetting* Setting = WorkstationComponent->GetProductionSetting(SettingIndex);

	if (!Setting)
		return;

	FWorkstationProductionSetting ModifiedSetting = *Setting;
	ModifiedSetting.Priority = FMath::RoundToInt(Value);
	WorkstationComponent->SetProductionSetting(ModifiedSetting);
}

void UWorkstationMenuRowWidget::RefreshVisuals()
{
	if (!WorkstationComponent || !Checkmark)
		return;

	const FWorkstationProductionSetting* Setting = WorkstationComponent->GetProductionSetting(SettingIndex);

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