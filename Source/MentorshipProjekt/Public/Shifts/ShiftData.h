// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShiftData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FShiftData
{
	GENERATED_BODY()

	//UPROPERTY()
	//int32 ShiftID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartHour = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EndHour = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AssignedArea; //FName is faster for comparisions and better suited for identifiers as FString

	FShiftData() {}
	FShiftData(const float InStart, const float InEnd) : StartHour(InStart), EndHour(InEnd) {}
	
	bool operator==(const FShiftData& Other) const
	{
		return StartHour == Other.StartHour && EndHour == Other.EndHour && AssignedArea == Other.AssignedArea;
	}
};