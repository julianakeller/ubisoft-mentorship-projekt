// Fill out your copyright notice in the Description page of Project Settings.


#include "FamilySimulationSubsystem.h"

#include "EngineUtils.h"
#include "MentorshipProjekt/GameTime/GameTimeSubsystem.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyMemberCharacter.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"
#include "MentorshipProjekt/NPCs/FamilyMember/FamilyInstanceData.h"
#include "MentorshipProjekt/NPCs/SpawnPoints/FamilyMemberSpawnPoint.h"

void UFamilySimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	
	
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("Subsystem Init World: %s"), *GetWorld()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("FamilySimulationSubsystem initialized"));
	
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		this,
		&UFamilySimulationSubsystem::InitializeFamilyMembers,
		0.5f,
		false
	);
}

#pragma region Initializing Family Member Instances

// Gets all family member data assets and uses them to initialize family member instances
void UFamilySimulationSubsystem::InitializeFamilyMembers()
{
	if (bInitializedFamilyMembers)
	{
		return;
	}

	FamilyMembers.Empty();
	bInitializedFamilyMembers = true;
	
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	
	AssetRegistry.WaitForCompletion();
	
	FARFilter FilterFamilyMemberDataAssets;
	FilterFamilyMemberDataAssets.ClassPaths.Add(UFamilyMemberDataAsset::StaticClass()->GetClassPathName());
	FilterFamilyMemberDataAssets.bRecursivePaths = true;
	FilterFamilyMemberDataAssets.bRecursiveClasses = true;
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
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FamilySimulationSubsystem: Cast unsuccessful."));
		}
	}
	
	if (FamilyMembers.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("FamilySimulationSubsystem: No Family Member Data Assets found."));
	}
}

void UFamilySimulationSubsystem::AddFamilyMember(const UFamilyMemberDataAsset* DataAsset)
{
	/*const int32 Index = FamilyMembers.AddDefaulted();
	FamilyMembers[Index].InitializeFromDataAsset(DataAsset);*/
	
	FFamilyInstanceData NewMember;
	NewMember.InitializeFromDataAsset(DataAsset);

	const FGuid Id = FGuid::NewGuid();
	FamilyMembers.Add(Id, NewMember);

	SpawnFamilyMemberCharacter(Id, DataAsset);
}

void UFamilySimulationSubsystem::SpawnFamilyMemberCharacter(const FGuid& Id, const UFamilyMemberDataAsset* DataAsset)
{
	if (!DataAsset || !DataAsset->CharacterClass)
	{
		return;
	}
	
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (FamilyMemberSpawnPoints.Num() == 0)
	{
		CacheSpawnPoints(World);
	}

	if (FamilyMemberSpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCustomerSimulationSubsystem: No CustomerSpawnPoints found"));
		return;
	}
	
	AActor* SpawnPoint = FamilyMemberSpawnPoints[FMath::RandRange(0, FamilyMemberSpawnPoints.Num() - 1)];

	AFamilyMemberCharacter* Character = World->SpawnActorDeferred<AFamilyMemberCharacter>(DataAsset->CharacterClass, SpawnPoint->GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	if (Character)
	{
		FFamilyInstanceData* Instance = FamilyMembers.Find(Id);
		Character->Initialize(Instance, Id);
		
		UGameplayStatics::FinishSpawningActor(Character, SpawnPoint->GetActorTransform());
	}
}

#pragma endregion

#pragma region Updating Family Members per Ticks

void UFamilySimulationSubsystem::OnMinuteChanged(const FInGameTime& NewTime)
{
	AccumulatedMinutes += 1.f;

	constexpr float StepSize = 10.f; // 6 updates per hour

	while (AccumulatedMinutes >= StepSize)
	{
		for (TPair<FGuid, FFamilyInstanceData>& Pair : FamilyMembers)
		{
			FFamilyInstanceData& Member = Pair.Value;
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
	constexpr float EnergyDecay = -0.2f;
	//constexpr float EnergyDecay = -100.f; // ToDo Testing
	constexpr float SocialDecay = -0.1f;
	//constexpr float SocialDecay = -100.f; // ToDo Testing
	constexpr float RecreationDecay = -0.15f;
	//constexpr float RecreationDecay = -100.f; // ToDo Testing

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

#pragma region Family Member Spawning

void UFamilySimulationSubsystem::CacheSpawnPoints(UWorld* World)
{
	FamilyMemberSpawnPoints.Empty();

	for (TActorIterator<AActor> It(World, AFamilyMemberSpawnPoint::StaticClass()); It; ++It)
	{
		FamilyMemberSpawnPoints.Add(*It);
	}
}

#pragma endregion