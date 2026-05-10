// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaSignWidget.h"

void UAreaSignWidget::SetName(FName AreaNameToSet)
{
	if (AreaName)
	{
		AreaName->SetText(FText::FromName(AreaNameToSet));
	}
}

void UAreaSignWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	AreaName->SetText(FText::FromName(NameToSet));
}
