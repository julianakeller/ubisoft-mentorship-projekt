// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MentorshipProjekt/GameTime/GameTimeSubsystem.h"
#include "NPCSimulationSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyInstanceData.h"
#include "FamilySimulationSubsystem.generated.h"

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

	UPROPERTY()
	TMap<FGuid, FFamilyInstanceData> FamilyMembers;
	
private:
	
	void InitializeFamilyMembers();
	
	void AddFamilyMember(const UFamilyMemberDataAsset* DataAsset);
	
	void SpawnFamilyMemberCharacter(const FGuid& Id, const UFamilyMemberDataAsset* DataAsset);
	
	void SimulateFamilyMember(FFamilyInstanceData& FamilyMember, const FInGameTime& NewTime, FInGameTime& LastUpdate);
	
	void LogFamilyMemberExtremeStats(FFamilyInstanceData& FamilyMember);
	
	float AccumulatedMinutes = 0.0f;
	
	void CacheSpawnPoints(UWorld* World);
	
	UPROPERTY()
	TArray<AActor*> FamilyMemberSpawnPoints;
	
	bool bInitializedFamilyMembers = false;
};
