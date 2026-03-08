// Fill out your copyright notice in the Description page of Project Settings.


#include "LogEntryWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void ULogEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULogEntryWidget::InitializeLogEntry(const FLogMessage& InMessage)
{
	LogMessage = InMessage;

	if (MessageText)
	{
		MessageText->SetText(LogMessage.Message);
	}

	ApplyColor();
	StartLifeTimer();
}

void ULogEntryWidget::ApplyColor()
{
	if (!MessageText) return;

	FSlateColor Color = FSlateColor(FLinearColor::White);

	switch (LogMessage.Type)
	{
	case ELogMessageType::Warning:
		Color = FSlateColor(FLinearColor(1.f, 0.5f, 0.f));
		break;

	case ELogMessageType::Important:
		Color = FSlateColor(FLinearColor(1.f, 0.1f, 0.f));
		break;

	default:
		Color = FSlateColor(FLinearColor(1.f, 1.f, 1.f));
		break;
	}

	MessageText->SetColorAndOpacity(Color);
}

void ULogEntryWidget::StartLifeTimer()
{
	if (LogMessage.Lifetime <= 0.f) return;

	GetWorld()->GetTimerManager().SetTimer(
		LifeTimerHandle,
		this,
		&ULogEntryWidget::HandleFadeOutFinished,
		LogMessage.Lifetime,
		false
	);
}

void ULogEntryWidget::HandleFadeOutFinished()
{
	if (FadeOutAnimation)
	{
		PlayAnimation(FadeOutAnimation);
		FWidgetAnimationDynamicEvent AnimationFinishedEvent;
		AnimationFinishedEvent.BindDynamic(this, &ULogEntryWidget::OnFadeAnimationFinished);

		BindToAnimationFinished(FadeOutAnimation, AnimationFinishedEvent);
	}
	else
	{
		RemoveFromParent();
	}
}

void ULogEntryWidget::OnFadeAnimationFinished()
{
	RemoveFromParent();
}