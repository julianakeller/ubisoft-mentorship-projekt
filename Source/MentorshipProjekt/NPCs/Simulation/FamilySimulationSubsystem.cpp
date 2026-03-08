// Fill out your copyright notice in the Description page of Project Settings.


#include "FamilySimulationSubsystem.h"
#include "GameTimeSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "MentorshipProjekt/NPCs/FamilyInstanceData.h"

void UFamilySimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	FCoreDelegates::OnPostEngineInit.AddLambda([this]()
	{
		InitializeFamilyMembers();
	});
}

#pragma region Initializing Family Member Instances

// Gets all family member data assets and uses them to initialize family member instances
void UFamilySimulationSubsystem::InitializeFamilyMembers()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	
	AssetRegistry.WaitForCompletion();
	
	FARFilter FilterFamilyMemberDataAssets;
	FilterFamilyMemberDataAssets.ClassPaths.Add(UFamilyMemberDataAsset::StaticClass()->GetClassPathName());
	FilterFamilyMemberDataAssets.bRecursivePaths = true;
	FilterFamilyMemberDataAssets.PackagePaths.Add("/Game/Data/FamilyMembers"); 

	TArray<FAssetData> AssetListFamilyMembers;
	AssetRegistry.GetAssets(FilterFamilyMemberDataAssets, AssetListFamilyMembers);
	
	for (const FAssetData& Data : AssetListFamilyMembers)
	{
		FSoftObjectPath AssetPath = Data.ToSoftObjectPath();
		
		if (UFamilyMemberDataAsset* Asset = Cast<UFamilyMemberDataAsset>(AssetPath.TryLoad()))
		{
			UE_LOG(LogTemp, Log, TEXT("Loaded Family Member: %s"), *Asset->FamilyMemberName.ToString()); //FName::ToString() returns an FString. FString overloads operator *() to return const TCHAR*. %s in UE_LOG expects const TCHAR*.
			AddFamilyMember(Asset);
		}
	}
	
	if (FamilyMembers.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("FamilySimulationSubsystem: No Family Member Data Assets found."));
	}
}

void UFamilySimulationSubsystem::AddFamilyMember(const UFamilyMemberDataAsset* DataAsset)
{
	const int32 Index = FamilyMembers.AddDefaulted();
	FamilyMembers[Index].InitializeFromDataAsset(DataAsset);
}

#pragma endregion

#pragma region Updating Family Members per Ticks

void UFamilySimulationSubsystem::OnMinuteChanged(const FInGameTime& NewTime)
{
	AccumulatedMinutes += 1.f;

	constexpr float StepSize = 10.f; // 6 updates per hour

	while (AccumulatedMinutes >= StepSize)
	{
		for (FFamilyInstanceData& Member : FamilyMembers)
		{
			SimulateFamilyMember(Member, NewTime, LastUpdateMinute);
		}
		LastUpdateMinute = NewTime;
		AccumulatedMinutes -= StepSize;
	}
}

void UFamilySimulationSubsystem::SimulateFamilyMember(FFamilyInstanceData& FamilyMember, const FInGameTime& NewTime, FInGameTime& LastUpdate)
{
	// ToDo set CurrentWorkArea in FamilyInstanceData
	
	// Needs decay:
	// ToDo: move decay rates to FamilyMemberDataAsset for individual (personality based) decay rates
	//constexpr float EnergyDecay = -0.2f;
	constexpr float EnergyDecay = -100.f; // ToDo Testing
	//constexpr float SocialDecay = -0.1f;
	constexpr float SocialDecay = -100.f; // ToDo Testing
	//constexpr float RecreationDecay = -0.15f;
	constexpr float RecreationDecay = -100.f; // ToDo Testing

	float DeltaMinutes = FInGameTime::DifferenceInMinutes(NewTime, LastUpdate);
	
	FamilyMember.UpdateEnergy(EnergyDecay * 1); // ToDo Use DeltaMinutes
	FamilyMember.UpdateSocial(SocialDecay * 1);
	FamilyMember.UpdateRecreation(RecreationDecay * 1);
	
	// Log low or high happiness and loyalty:
	LogFamilyMemberExtremeStats(FamilyMember);
}

void UFamilySimulationSubsystem::LogFamilyMemberExtremeStats(FFamilyInstanceData& FamilyMember)
{
	if (LogSubsystem)
	{
		if (FamilyMember.bHighLoyalty)
		{
			if (!FamilyMember.bLoggedLoyalty)
			{
				LogSubsystem->LogFamilyMemberHighLoyalty(FamilyMember);
				FamilyMember.bLoggedLoyalty = true;
			}
		}
		else if (FamilyMember.bLowLoyalty)
		{
			if (!FamilyMember.bLoggedLoyalty)
			{
				LogSubsystem->LogFamilyMemberLowLoyalty(FamilyMember);
				FamilyMember.bLoggedLoyalty = true;
			}
		}
		else
		{
			FamilyMember.bLoggedLoyalty = false;
		}
		
		if (FamilyMember.bHighHappiness)
		{
			if (!FamilyMember.bLoggedHappiness)
			{
				LogSubsystem->LogFamilyMemberHighHappiness(FamilyMember);
				FamilyMember.bLoggedHappiness = true;
			}
		}
		else if (FamilyMember.bLowHappiness)
		{
			if (!FamilyMember.bLoggedHappiness)
			{
				LogSubsystem->LogFamilyMemberLowHappiness(FamilyMember);
				FamilyMember.bLoggedHappiness = true;
			}
		}
		else
		{
			FamilyMember.bLoggedHappiness = false;
		}
	}
}

#pragma endregion