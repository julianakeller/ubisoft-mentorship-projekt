// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorkerTimelineWidget.generated.h"

/**
 * 
 */

class UShiftManager;
class UCanvasPanel;
class UShiftBlockWidget;

UCLASS()
class MENTORSHIPPROJEKT_API UWorkerTimelineWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* TimelineCanvas;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UShiftBlockWidget> ShiftBlockClass;
	
	bool IsShiftOverlapping(const float Start, const float End, const UShiftBlockWidget* IgnoreShift) const;
	
	// Worker ID for this timeline
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn=true))
	FName WorkerID;
	
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	bool bCreatingShift = false;
	float CreationStartHour;
	UShiftBlockWidget* TempShift;

	float GetHourFromLocalX(float LocalX, float Width) const;
	bool IsOverExistingShift(float Hour) const;
	
	UFUNCTION()
	void LogShifts() const;
	
	UPROPERTY()
	UShiftManager* ShiftManager;
};
