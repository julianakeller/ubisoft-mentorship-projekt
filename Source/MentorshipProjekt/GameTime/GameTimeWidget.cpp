// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimeWidget.h"
#include "Components/TextBlock.h"
#include "GameTimeSubsystem.h"

void UGameTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameTimeSubsystem* TimeSystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>();

	if (TimeSystem)
	{
		TimeSystem->OnMinuteChanged.AddDynamic(this, &UGameTimeWidget::HandleTimeUpdated);

		HandleTimeUpdated(TimeSystem->GetCurrentTime());
		
		if (PauseButton)
		{
			PauseButton->OnClicked.AddDynamic(this, &UGameTimeWidget::OnPausedClicked);
		}

		if (NormalSpeedButton)
		{
			NormalSpeedButton->OnClicked.AddDynamic(this, &UGameTimeWidget::OnNormalClicked);
		}

		if (FastSpeedButton)
		{
			FastSpeedButton->OnClicked.AddDynamic(this, &UGameTimeWidget::OnFastClicked);
		}

		if (VeryFastSpeedButton)
		{
			VeryFastSpeedButton->OnClicked.AddDynamic(this, &UGameTimeWidget::OnVeryFastClicked);
		}
	}
}

void UGameTimeWidget::HandleTimeUpdated(const FInGameTime& NewTime)
{
	if (Text_Time)
	{
		Text_Time->SetText(FText::FromString(NewTime.ToFormattedString()));
	}
}

void UGameTimeWidget::OnPausedClicked()
{
	if (UGameTimeSubsystem* TimeSystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>())
	{
		TimeSystem->SetTimeSpeed(ETimeSpeed::Paused);
	}
}

void UGameTimeWidget::OnNormalClicked()
{
	if (UGameTimeSubsystem* TimeSystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>())
	{
		TimeSystem->SetTimeSpeed(ETimeSpeed::Normal);
	}
}

void UGameTimeWidget::OnFastClicked()
{
	if (UGameTimeSubsystem* TimeSystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>())
	{
		TimeSystem->SetTimeSpeed(ETimeSpeed::Fast);
	}
}

void UGameTimeWidget::OnVeryFastClicked()
{
	if (UGameTimeSubsystem* TimeSystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>())
	{
		TimeSystem->SetTimeSpeed(ETimeSpeed::VeryFast);
	}
}
