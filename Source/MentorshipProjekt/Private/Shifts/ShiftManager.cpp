// Fill out your copyright notice in the Description page of Project Settings.


#include "Shifts/ShiftManager.h"

FWorkerSchedule* UShiftManager::GetWorkerSchedule(FName WorkerID)
{
	for (FWorkerSchedule& Schedule : WorkerSchedules)
	{
		if (Schedule.WorkerID == WorkerID)
			return &Schedule;
	}
	return nullptr;
}

bool UShiftManager::RemoveShift(FName WorkerID, int32 ShiftIndex)
{
	FWorkerSchedule* Schedule = GetWorkerSchedule(WorkerID);
    if (!Schedule) return false;

    if (!Schedule->Shifts.IsValidIndex(ShiftIndex))
        return false;

    Schedule->Shifts.RemoveAt(ShiftIndex);
    return true;
}

bool UShiftManager::UpdateShift(FName WorkerID, int32 ShiftIndex, float NewStart, float NewEnd)
{
	FWorkerSchedule* Schedule = GetWorkerSchedule(WorkerID);
	if (!Schedule) return false;

	if (!Schedule->Shifts.IsValidIndex(ShiftIndex))
		return false;
	
	Schedule->Shifts[ShiftIndex].StartHour = NewStart;
	Schedule->Shifts[ShiftIndex].EndHour = NewEnd;

	return true;
}

bool UShiftManager::AddWorkerIfMissing(FName WorkerID)
{
	if (GetWorkerSchedule(WorkerID) == nullptr)
	{
		WorkerSchedules.Add(FWorkerSchedule(WorkerID));
		return true;
	}
	return false;
}

FShiftData* UShiftManager::AddShift(FName WorkerID, FShiftData& NewShift)
{
	AddWorkerIfMissing(WorkerID);

	FWorkerSchedule* Schedule = GetWorkerSchedule(WorkerID);
	if (!Schedule)
		return nullptr;

	const int32 ShiftIndex = Schedule->Shifts.Add(NewShift);

	LogShifts();
	
	return &Schedule->Shifts[ShiftIndex];
}

FShiftData* UShiftManager::AddNewShift(FName WorkerID, float Start, float End)
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
			UE_LOG(LogTemp, Display, TEXT("Shift: Start: %.2f, End: %.2f"), Shift.StartHour, Shift.EndHour);
		}
	}
}