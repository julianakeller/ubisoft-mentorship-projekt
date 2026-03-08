// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimeWidget.h"
#include "Components/TextBlock.h"
#include "GameTimeSubsystem.h"

void UGameTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameTimeSubsystem* TimeSystem =
		GetWorld()->GetSubsystem<UGameTimeSubsystem>();

	if (TimeSystem)
	{
		TimeSystem->OnMinuteChanged.AddDynamic(
			this,
			&UGameTimeWidget::HandleTimeUpdated
		);

		HandleTimeUpdated(TimeSystem->GetCurrentTime());
	}
}

void UGameTimeWidget::HandleTimeUpdated(const FInGameTime& NewTime)
{
	if (Text_Time)
	{
		Text_Time->SetText(FText::FromString(NewTime.ToFormattedString()));
	}
}