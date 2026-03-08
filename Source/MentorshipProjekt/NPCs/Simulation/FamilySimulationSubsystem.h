// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameTimeSubsystem.h"
#include "NPCSimulationSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "FamilySimulationSubsystem.generated.h"

struct FFamilyInstanceData;
class UFamilyMemberDataAsset;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UFamilySimulationSubsystem : public UNPCSimulationSubsystem
{
	GENERATED_BODY()
	
public:
	// Initializes subsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void OnMinuteChanged(const FInGameTime& NewTime) override;

private:
	UPROPERTY()
	TArray<FFamilyInstanceData> FamilyMembers;
	
	void InitializeFamilyMembers();
	
	void AddFamilyMember(const UFamilyMemberDataAsset* DataAsset);
	
	void SimulateFamilyMember(FFamilyInstanceData& FamilyMember, const FInGameTime& NewTime, FInGameTime& LastUpdate);
	
	void LogFamilyMemberExtremeStats(FFamilyInstanceData& FamilyMember);
	
	float AccumulatedMinutes = 0.0f;
};
