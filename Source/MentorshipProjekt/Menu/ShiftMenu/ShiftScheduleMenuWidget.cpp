// Fill out your copyright notice in the Description page of Project Settings.


#include "ShiftScheduleMenuWidget.h"
#include "MentorshipProjekt/NPCs/Simulation/FamilySimulationSubsystem.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyInstanceData.h"
#include "MentorshipProjekt/Public/Shifts/ShiftManager.h"
#include "WorkAreaButtonWidget.h"

// ToDo automatically fill menu with a worker row widget per family member in family simulation subsystem

void UShiftScheduleMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UWorld* World = GetWorld())
	{
		FamilySimulationSubsystem = World->GetSubsystem<UFamilySimulationSubsystem>();
		ShiftManager = World->GetSubsystem<UShiftManager>();
		WorkAreaManager = World->GetSubsystem<UWorkAreaManager>();
	}
}

void UShiftScheduleMenuWidget::OnMenuOpened()
{
	Super::OnMenuOpened();
	
	if (FamilySimulationSubsystem)
	{
		PopulateWorkerRows();
		PopulateWorkAreaButtons();
	}
}

void UShiftScheduleMenuWidget::PopulateWorkAreaButtons()
{
	if (!WorkAreaManager || !WorkAreaButtonContainer || !WorkAreaButtonClass)
		return;

	WorkAreaButtonContainer->ClearChildren();

	for (AWorkAreaBase* Area : WorkAreaManager->GetAllWorkAreas())
	{
		if (!Area) continue;

		UWorkAreaButtonWidget* Button = CreateWidget<UWorkAreaButtonWidget>(this, WorkAreaButtonClass);

		WorkAreaButtons.Add(Button);
		
		Button->InitializeButton(Area->WorkAreaName, Area->WorkAreaColor);

		Button->OnClicked.AddDynamic(this, &UShiftScheduleMenuWidget::HandleWorkAreaClicked);

		WorkAreaButtonContainer->AddChild(Button);
	}
}

void UShiftScheduleMenuWidget::HandleWorkAreaClicked(FName AreaName)
{
	SelectedWorkArea = AreaName;
	
	for (UWorkAreaButtonWidget* WorkAreaButton : WorkAreaButtons)
	{
		if (WorkAreaButton->GetAreaName() != AreaName)
		{
			WorkAreaButton->SetSelected(false);
		}
	}
}

void UShiftScheduleMenuWidget::PopulateWorkerRows()
{
	if (!WorkerRowContainer || !WorkerRowWidget || !FamilySimulationSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShiftScheduleMenuWidget: Cannot populate worker rows. Container, widget class or subsystem is missing."));
		return;
	}

	WorkerRowContainer->ClearChildren();

	// Iterate over all family members:
	for (const TPair<FGuid, FFamilyInstanceData>& Pair : FamilySimulationSubsystem->FamilyMembers)
	{
		const FGuid& WorkerID = Pair.Key;
		const FFamilyInstanceData& InstanceData = Pair.Value;

		if (!InstanceData.FamilyMemberData)
		{
			UE_LOG(LogTemp, Warning, TEXT("ShiftScheduleMenuWidget: FamilyMemberData is null for WorkerID %s"), *WorkerID.ToString());
			continue;
		}

		// Create WorkerRowWidget
		if (UWorkerRowWidget* RowWidget = CreateWidget<UWorkerRowWidget>(this, WorkerRowWidget))
		{
			RowWidget->InitializeRow(WorkerID, FText::FromName(InstanceData.FamilyMemberData->FamilyMemberName), this);

			WorkerRowContainer->AddChild(RowWidget);
		}
	}
}