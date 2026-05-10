// Fill out your copyright notice in the Description page of Project Settings.


#include "Shifts/ShiftManager.h"

#include "MentorshipProjekt/GameTime/GameTimeSubsystem.h"

void UShiftManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	GameTimeSubsystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>();
	
	GameTimeSubsystem->OnHourChanged.AddDynamic(this, &UShiftManager::TickShifts);
}

FWorkerSchedule* UShiftManager::GetWorkerSchedule(FGuid WorkerID)
{
	for (FWorkerSchedule& Schedule : WorkerSchedules)
	{
		if (Schedule.WorkerID == WorkerID)
			return &Schedule;
	}
	return nullptr;
}

bool UShiftManager::RemoveShift(FGuid WorkerID, int32 ShiftIndex)
{
	FWorkerSchedule* Schedule = GetWorkerSchedule(WorkerID);
	if (!Schedule)
	{
		return false;
	}

	if (!Schedule->Shifts.IsValidIndex(ShiftIndex)){
		return false;
	}
	
	if (CurrentActiveShiftIndex.Contains(WorkerID) && CurrentActiveShiftIndex[WorkerID] == ShiftIndex)
	{
		CurrentActiveShiftIndex.Add(WorkerID, INDEX_NONE);
		OnShiftStateChanged.Broadcast(WorkerID, nullptr);
	}

    Schedule->Shifts.RemoveAt(ShiftIndex);
    return true;
}

bool UShiftManager::UpdateShift(FGuid WorkerID, int32 ShiftIndex, float NewStart, float NewEnd)
{
	FWorkerSchedule* Schedule = GetWorkerSchedule(WorkerID);
	if (!Schedule || !Schedule->Shifts.IsValidIndex(ShiftIndex))
	{
		return false;
	}
	
	Schedule->Shifts[ShiftIndex].StartHour = NewStart;
	Schedule->Shifts[ShiftIndex].EndHour = NewEnd;
	
	// ToDo update currently active shifts?

	// Broadcast event if updated shift is currently active:
	if (CurrentActiveShiftIndex.Contains(WorkerID) && CurrentActiveShiftIndex[WorkerID] == ShiftIndex)
	{
		OnShiftStateChanged.Broadcast(WorkerID, &Schedule->Shifts[ShiftIndex]);
		UE_LOG(LogTemp, Display, TEXT("Broadcasted OnShiftStateChanged"));
	}
	
	return true;
}

void UShiftManager::TickShifts(const FInGameTime& NewTime)
{
	int32 CurrentHour = NewTime.Hour;
	
	for (const FWorkerSchedule& Schedule : WorkerSchedules)
	{
		const FShiftData* ActiveShift = GetCurrentShift(Schedule.WorkerID, CurrentHour);

		int32 PreviousIndex = CurrentActiveShiftIndex.Contains(Schedule.WorkerID) ? CurrentActiveShiftIndex[Schedule.WorkerID] : INDEX_NONE;
		int32 NewIndex = ActiveShift ? Schedule.Shifts.IndexOfByKey(*ActiveShift) : INDEX_NONE;

		// Shift changed
		if (PreviousIndex != NewIndex)
		{
			CurrentActiveShiftIndex.Add(Schedule.WorkerID, NewIndex);
			OnShiftStateChanged.Broadcast(Schedule.WorkerID, ActiveShift);
			UE_LOG(LogTemp, Display, TEXT("Shift has changed"));
		}
	}
}

FShiftData* UShiftManager::GetCurrentShift(FGuid WorkerID, int32 CurrentHour)
{
	FWorkerSchedule* Schedule = GetWorkerSchedule(WorkerID);
	if (!Schedule || Schedule->Shifts.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetCurrentShift: No schedule or empty shifts for Worker %s"), *WorkerID.ToString());
		return nullptr;
	}

	for (FShiftData& Shift : Schedule->Shifts)
	{
		if (Shift.StartHour <= CurrentHour && CurrentHour < Shift.EndHour)
		{
			return &Shift;
		}
	}
	return nullptr;
}

bool UShiftManager::AddWorkerIfMissing(FGuid WorkerID)
{
	if (GetWorkerSchedule(WorkerID) == nullptr)
	{
		WorkerSchedules.Add(FWorkerSchedule(WorkerID));
		return true;
	}
	return false;
}

FShiftData* UShiftManager::AddShift(FGuid WorkerID, FShiftData& NewShift)
{
	AddWorkerIfMissing(WorkerID);

	FWorkerSchedule* Schedule = GetWorkerSchedule(WorkerID);
	if (!Schedule)
	{
		return nullptr;
	}

	const int32 ShiftIndex = Schedule->Shifts.Add(NewShift);

	LogShifts();
	
	//Broadcast if shift is already active:
	if (GameTimeSubsystem)
	{
		const int32 CurrentHour = GameTimeSubsystem->GetCurrentTime().Hour;
		FShiftData* AddedShift = &Schedule->Shifts[ShiftIndex];
		if (AddedShift->StartHour <= CurrentHour && CurrentHour < AddedShift->EndHour)
		{
			CurrentActiveShiftIndex.Add(WorkerID, ShiftIndex);
			OnShiftStateChanged.Broadcast(WorkerID, AddedShift);
			UE_LOG(LogTemp, Display, TEXT("Broadcasted OnShiftStateChanged for newly added active shift"));
		}
	}
	
	return &Schedule->Shifts[ShiftIndex];
}

FShiftData* UShiftManager::AddNewShift(FGuid WorkerID, float Start, float End)
{
	FShiftData NewShift;
	NewShift.StartHour = Start;
	NewShift.EndHour = End;
	
	return AddShift(WorkerID, NewShift);
}

void UShiftManager::LogShifts()
{
	UE_LOG(LogTemp, Display, TEXT("Schedule Manager Shifts:"));
	for (FWorkerSchedule Schedule : WorkerSchedules)
	{
		UE_LOG(LogTemp, Display, TEXT("Worker: %s"), *Schedule.WorkerID.ToString())
		for (const FShiftData Shift : Schedule.Shifts)
		{
			UE_LOG(LogTemp, Display, TEXT("Shift: Area: %s"), *Shift.AssignedArea.ToString());
			UE_LOG(LogTemp, Display, TEXT("Shift: Start: %.2f, End: %.2f"), Shift.StartHour, Shift.EndHour);
		}
	}
}