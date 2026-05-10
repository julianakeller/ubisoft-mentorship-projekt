// Fill out your copyright notice in the Description page of Project Settings.


#include "FamilyEconomySubsystem.h"
#include "MentorshipProjekt/GameTime/GameTimeSubsystem.h"

void UFamilyEconomySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (UWorld* World = GetWorld())
	{
		TimeSubsystem = World->GetSubsystem<UGameTimeSubsystem>();

		if (TimeSubsystem)
		{
			TimeSubsystem->OnHourChanged.AddDynamic(this, &UFamilyEconomySubsystem::HandleHourChanged);
			TimeSubsystem->OnDayChanged.AddDynamic(this, &UFamilyEconomySubsystem::HandleDayChanged);
		}
	}
}

void UFamilyEconomySubsystem::Deinitialize()
{
	if (TimeSubsystem)
	{
		TimeSubsystem->OnHourChanged.RemoveAll(this);
		TimeSubsystem->OnDayChanged.RemoveAll(this);
	}
	
	Super::Deinitialize();
}

void UFamilyEconomySubsystem::AddGold(float Amount)
{
	Gold += Amount;
	
	if (Amount >= 0.f)
	{
		CurrentHourIncome+=Amount;
	}
	else
	{
		CurrentHourExpenses += FMath::Abs(Amount);
	}
	
	OnGoldChanged.Broadcast();
}

void UFamilyEconomySubsystem::AddReputation(float Amount)
{
	Reputation += Amount;
	
	CurrentHourReputation+=Amount;
	
	OnReputationChanged.Broadcast();
}

#pragma region Time Events

void UFamilyEconomySubsystem::HandleHourChanged(const FInGameTime& NewTime)
{
	FinalizeHour();
}

void UFamilyEconomySubsystem::HandleDayChanged(const FInGameTime& NewTime)
{
	FinalizeDay();
}

void UFamilyEconomySubsystem::FinalizeHour()
{
	FEconomySnapshot Snapshot;
	Snapshot.Income = CurrentHourIncome;
	Snapshot.Expenses = CurrentHourExpenses;
	Snapshot.ReputationChange = CurrentHourReputation;

	HourlyStats.Add(Snapshot);
	
	TrimArray(HourlyStats, MaxHourlyEntries);

	// Reset accumulations:
	CurrentHourIncome = 0.f;
	CurrentHourExpenses = 0.f;
	CurrentHourReputation = 0.f;
}

void UFamilyEconomySubsystem::FinalizeDay()
{
	FEconomySnapshot DailySnapshot;

	const int32 NumHours = HourlyStats.Num();
	const int32 StartIndex = FMath::Max(0, NumHours - 24);

	for (int32 i = StartIndex; i < NumHours; i++)
	{
		DailySnapshot.Income += HourlyStats[i].Income;
		DailySnapshot.Expenses += HourlyStats[i].Expenses;
		DailySnapshot.ReputationChange += HourlyStats[i].ReputationChange;
	}

	DailyStats.Add(DailySnapshot);
	
	TrimArray(DailyStats, MaxDailyEntries);
}

template<typename T>
void UFamilyEconomySubsystem::TrimArray(TArray<T>& Array, int32 MaxSize)
{
	if (MaxSize <= 0) return;

	if (Array.Num() > MaxSize)
	{
		const int32 NumToRemove = Array.Num() - MaxSize;
		Array.RemoveAt(0, NumToRemove, EAllowShrinking::No);
	}
}

#pragma endregion