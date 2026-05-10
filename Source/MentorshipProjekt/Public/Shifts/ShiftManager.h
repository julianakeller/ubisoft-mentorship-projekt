// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ShiftData.h"
#include "ShiftManager.generated.h"

/**
 * 
 */

struct FInGameTime;
class UGameTimeSubsystem;

USTRUCT(BlueprintType)
struct FWorkerSchedule
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid WorkerID;  //FName is faster for comparisions and better suited for identifiers as FString

	UPROPERTY()
	TArray<FShiftData> Shifts;
	
	FWorkerSchedule() {}
	FWorkerSchedule(const FGuid InWorkerID) : WorkerID(InWorkerID) {}
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnShiftStateChanged, FGuid, const FShiftData*); // WorkerId and new Shift as parameters

UCLASS()
class MENTORSHIPPROJEKT_API UShiftManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	
	FOnShiftStateChanged OnShiftStateChanged;
	
	FWorkerSchedule* GetWorkerSchedule(FGuid WorkerID);
	
	FShiftData* AddShift(FGuid WorkerID, FShiftData& NewShift);
	
	FShiftData* AddNewShift(FGuid WorkerID, float Start, float End);

	UFUNCTION(BlueprintCallable)
	bool RemoveShift(FGuid WorkerID, int32 ShiftIndex);

	UFUNCTION(BlueprintCallable)
	bool UpdateShift(FGuid WorkerID, int32 ShiftIndex, float NewStart, float NewEnd);
	
	UFUNCTION(BlueprintCallable)
	bool AddWorkerIfMissing(FGuid WorkerID);
	
	FShiftData* GetCurrentShift(FGuid WorkerID, int32 CurrentHour);

	UFUNCTION()
	void TickShifts(const FInGameTime& NewTime);
	
	//UFUNCTION(BlueprintCallable)
	//bool AssignWorkAreaToShift(FName WorkerID, FShiftData Shift, FName WorkAreaID);
	
private:
	
	UPROPERTY(VisibleAnywhere)
	TArray<FWorkerSchedule> WorkerSchedules;
	
	// Tracks which shift is currently active for each worker
	TMap<FGuid, int32> CurrentActiveShiftIndex;
	
	void LogShifts();
	
	UGameTimeSubsystem* GameTimeSubsystem;
};
