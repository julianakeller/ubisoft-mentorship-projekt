// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ShiftData.h"
#include "ShiftManager.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWorkerSchedule
{
	GENERATED_BODY()

	UPROPERTY()
	FName WorkerID;  //FName is faster for comparisions and better suited for identifiers as FString

	UPROPERTY()
	TArray<FShiftData> Shifts;
	
	FWorkerSchedule() {}
	FWorkerSchedule(const FName InWorkerID) : WorkerID(InWorkerID) {}
};

UCLASS()
class MENTORSHIPPROJEKT_API UShiftManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	FWorkerSchedule* GetWorkerSchedule(FName WorkerID);
	
	FShiftData* AddShift(FName WorkerID, FShiftData& NewShift);
	
	FShiftData* AddNewShift(FName WorkerID, float Start, float End);

	UFUNCTION(BlueprintCallable)
	bool RemoveShift(FName WorkerID, int32 ShiftIndex);

	UFUNCTION(BlueprintCallable)
	bool UpdateShift(FName WorkerID, int32 ShiftIndex, float NewStart, float NewEnd);
	
	UFUNCTION(BlueprintCallable)
	bool AddWorkerIfMissing(FName WorkerID);
	
	//UFUNCTION(BlueprintCallable)
	//bool AssignWorkAreaToShift(FName WorkerID, FShiftData Shift, FName WorkAreaID);
	
private:

	//UPROPERTY()
	//int32 NextShiftID = 1;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FWorkerSchedule> WorkerSchedules;
	
	void LogShifts();
};
