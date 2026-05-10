// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WorkerRowWidget.generated.h"

class UShiftScheduleMenuWidget;
class UWorkerTimelineWidget;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UWorkerRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct();
	
	void InitializeRow(const FGuid& InWorkerID, const FText& InName, UShiftScheduleMenuWidget* InMenu);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterNameText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FGuid WorkerID;
	
protected:
	
	//BindWidget -> worker timeline will be automatically assigned if name matches "WorkerTimeline"
	UPROPERTY(meta = (BindWidget))
	UWorkerTimelineWidget* WorkerTimeline;
};
