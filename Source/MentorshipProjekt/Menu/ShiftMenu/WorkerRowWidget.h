// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorkerRowWidget.generated.h"

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
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FName WorkerID;
	
	//BindWidget -> worker timeline will be automatically assigned if name matches "WorkerTimeline"
	UPROPERTY(meta = (BindWidget))
	UWorkerTimelineWidget* WorkerTimeline;
};
