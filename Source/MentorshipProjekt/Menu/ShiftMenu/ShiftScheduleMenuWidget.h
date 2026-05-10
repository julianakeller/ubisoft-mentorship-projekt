// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorkerRowWidget.h"
#include "../BaseMenuWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "MentorshipProjekt/Areas/WorkAreaManager.h"
#include "ShiftScheduleMenuWidget.generated.h"

class UWorkAreaButtonWidget;
class UShiftManager;
class UFamilySimulationSubsystem;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UShiftScheduleMenuWidget : public UBaseMenuWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWorkerRowWidget> WorkerRowWidget;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* WorkerRowContainer;
	
	UPROPERTY()
	FName SelectedWorkArea;
	
	virtual void OnMenuOpened() override;
	
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* WorkAreaButtonContainer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWorkAreaButtonWidget> WorkAreaButtonClass;
	
protected:
	
	virtual void NativeConstruct() override;
	
private:
	
	TArray<TObjectPtr<UWorkAreaButtonWidget>> WorkAreaButtons;
	
	void PopulateWorkerRows();
	void PopulateWorkAreaButtons();
	
	UFUNCTION()
	void HandleWorkAreaClicked(FName AreaName);
	
	UFamilySimulationSubsystem* FamilySimulationSubsystem = nullptr;
	UShiftManager* ShiftManager = nullptr;
	UWorkAreaManager* WorkAreaManager = nullptr;
};
