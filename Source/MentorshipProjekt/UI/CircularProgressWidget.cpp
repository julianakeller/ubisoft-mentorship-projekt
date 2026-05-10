// Fill out your copyright notice in the Description page of Project Settings.


#include "CircularProgressWidget.h"
#include "Components/Image.h"

void UCircularProgressWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ProgressImage)
	{
		MID = ProgressImage->GetDynamicMaterial();
	}
	
	SetVisibility(ESlateVisibility::Collapsed);
}

void UCircularProgressWidget::SetProgress(float Value)
{
	if (MID)
	{
		MID->SetScalarParameterValue(TEXT("Progress"), FMath::Clamp(Value, 0.f, 1.f));
	}
}

void UCircularProgressWidget::EnableWidget()
{
	SetVisibility(ESlateVisibility::Visible);
	SetProgress(0);
}

void UCircularProgressWidget::DisableWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
