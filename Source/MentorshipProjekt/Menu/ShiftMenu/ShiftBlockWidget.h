// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "Shifts/ShiftData.h"
#include "ShiftBlockWidget.generated.h"

/**
 * 
 */

class UWorkAreaDropdownWidget;
class UWorkerTimelineWidget;

UENUM()
enum class EShiftInteractionMode : uint8
{
	None,
	ResizingLeft,
	ResizingRight
};

UCLASS()
class MENTORSHIPPROJEKT_API UShiftBlockWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeShift(const float InStartHour, const float InEndHour, const float InTimelineWidth, const float InTimelineHeight, UWorkerTimelineWidget* InParentTimeline);

	float GetStartHour() const { return StartHour; }
	float GetEndHour() const { return EndHour; }

	void SetStartHour(float NewStart);
	void SetEndHour(float NewEnd);
	
	static inline float SnapToQuarterHour(float Hour)
	{
		return FMath::Clamp(FMath::RoundToFloat(Hour/0.25f) * 0.25f, 0.f, 24.f);
	}
	
	FShiftData* GetLinkedShiftData() const {return LinkedShiftData;}
	void SetLinkedShiftData(FShiftData* NewLinkedShiftData) {LinkedShiftData = NewLinkedShiftData;};
	
	void InitializeShiftAppearance();
	
protected:
	virtual void NativeConstruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	UPROPERTY(meta=(BindWidget))
	UBorder* ShiftBorder;
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* ResizingIndicatorLeft;

	UPROPERTY(meta=(BindWidget))
	USizeBox* ResizingIndicatorRight;
	
private:
	float StartHour;
	float EndHour;
	float TimelineWidth;
	float TimelineHeight;

	float EdgeDetectionRatio = 0.1f;

	EShiftInteractionMode InteractionMode = EShiftInteractionMode::None;

	void UpdateVisual() const;
	void UpdateShiftAppearance();
	
	void UpdateResizingIndicator(float LocalX, float Width);
	
	UPROPERTY()
	UWorkerTimelineWidget* ParentTimeline = nullptr;
	
	FShiftData* LinkedShiftData = nullptr;
	
	/*UPROPERTY(meta=(BindWidget))
	UWorkAreaDropdownWidget* WorkAreaDropdown;*/
	
	UFUNCTION()
	void HandleWorkAreaSelected(FName SelectedArea);
	
	float GetEdgeDetectionWidth(float Width);
	
	void SetResizingIndicatorVisibility(bool bLeft, bool bRight);
	
	void DeleteShift();
};
