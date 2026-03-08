// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerSimulationSubsystem.h"

#include "GameTimeSubsystem.h"
#include "GameplayTagsManager.h"
#include "MPGameplayTags.h"
#include "MentorshipProjekt/NPCs/Customer/RelationshipRuleDataAsset.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerRelationship.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "LoggingSystem/LogSubsystem.h"

void UCustomerSimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	
	Super::Initialize(Collection);
	
	FCoreDelegates::OnPostEngineInit.AddLambda([this]()
	{
		LoadCustomerDataAssets();
	});
}

void UCustomerSimulationSubsystem::LoadCustomerDataAssets()
{
	//FAssetRegistryModule can check assets with filter created above
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	
	AssetRegistry.WaitForCompletion();
	
	// Load Relationship Rule Asset:
	FARFilter FilterRelationshipRules; //Filter specifies what the Asset Registry should look for
	FilterRelationshipRules.ClassPaths.Add(URelationshipRuleDataAsset::StaticClass()->GetClassPathName()); //Only look for RelationShipDataAsset
	FilterRelationshipRules.bRecursivePaths = true;
	FilterRelationshipRules.PackagePaths.Add("/Game/Data/Customers/Relationships"); //Folder to search

	//Store all assets found in the search for customer data assets:
	TArray<FAssetData> AssetListRelationshipRules; //FAssetData can be any asset in the content browser
	AssetRegistry.GetAssets(FilterRelationshipRules, AssetListRelationshipRules); //Fills AssetList with every FAssetData that matches filter

	if (AssetListRelationshipRules.Num() > 0)
	{
		FSoftObjectPath AssetPath = AssetListRelationshipRules[0].ToSoftObjectPath();
		RelationshipRulesAsset = Cast<URelationshipRuleDataAsset>(AssetPath.TryLoad());
	}
	
	// Load customer profession data assets:
	FARFilter FilterProfessions;
	FilterProfessions.ClassPaths.Add(UProfessionDataAsset::StaticClass()->GetClassPathName());
	FilterProfessions.bRecursivePaths = true;
	FilterProfessions.PackagePaths.Add("/Game/Data/Customers/Professions");

	TArray<FAssetData> AssetListProfessions;
	AssetRegistry.GetAssets(FilterProfessions, AssetListProfessions);
	
	for (const FAssetData& Data : AssetListProfessions)
	{
		FSoftObjectPath AssetPath = Data.ToSoftObjectPath();
		
		if (UProfessionDataAsset* Asset = Cast<UProfessionDataAsset>(AssetPath.TryLoad()))
		{
			ProfessionDataAssets.Add(Asset);
			UE_LOG(LogTemp, Log, TEXT("Loaded Customer Profession: %s"), *Asset->ProfessionName.ToString());
		}
		/*
		if (UProfessionDataAsset* Asset = Cast<UProfessionDataAsset>(Data.GetAsset()))
		{
			ProfessionDataAssets.Add(Asset);
			UE_LOG(LogTemp, Log, TEXT("Loaded Customer Profession: %s"), *Asset->ProfessionName.ToString()); //FName::ToString() returns an FString. FString overloads operator *() to return const TCHAR*. %s in UE_LOG expects const TCHAR*.
		}*/
	}
	
	// Load Health Condition Data Assets:
	
	FARFilter FilterHealthConditions;
	FilterHealthConditions.ClassPaths.Add(UHealthConditionDataAsset::StaticClass()->GetClassPathName());
	FilterHealthConditions.bRecursivePaths = true;
	FilterHealthConditions.PackagePaths.Add("/Game/Data/Customers/HealthConditions");

	TArray<FAssetData> AssetListHealthConditions;
	AssetRegistry.GetAssets(FilterHealthConditions, AssetListHealthConditions);
	
	for (const FAssetData& Data : AssetListHealthConditions)
	{
		FSoftObjectPath AssetPath = Data.ToSoftObjectPath();
		
		if (UHealthConditionDataAsset* Asset = Cast<UHealthConditionDataAsset>(AssetPath.TryLoad()))
		{
			HealthConditionDataAssets.Add(Asset);
			UE_LOG(LogTemp, Log, TEXT("Loaded Customer Health Condition: %s"), *Asset->ConditionName.ToString());
		}
		/*
		if (UHealthConditionDataAsset* Asset = Cast<UHealthConditionDataAsset>(Data.GetAsset()))
		{
			HealthConditionDataAssets.Add(Asset);
			UE_LOG(LogTemp, Log, TEXT("Loaded Customer Health Condition: %s"), *Asset->ConditionName.ToString()); //FName::ToString() returns an FString. FString overloads operator *() to return const TCHAR*. %s in UE_LOG expects const TCHAR*.
		}*/
	}
	
	// Notify of failures to load data assets:
	
	if (ProfessionDataAssets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CustomerSimulationSubsystem: No Profession Data Assets found."));
	}
	
	if (HealthConditionDataAssets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CustomerSimulationSubsystem: No Health Condition Data Assets found."));
	}

	if (!RelationshipRulesAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("CustomerSimulationSubsystem: No RelationshipRuleDataAsset found."));
	}
	else
	{
		BuildTransitionMap();
	}
}

void UCustomerSimulationSubsystem::OnMinuteChanged(const FInGameTime& NewTime)
{
	GenerateNewCustomer(NewTime, LastUpdateMinute);
	LastUpdateMinute = NewTime;
}

void UCustomerSimulationSubsystem::OnHourChanged(const FInGameTime& NewTime)
{
	for (TPair<FGuid, FCustomerInstanceData>& Pair : Customers)
	{
		FGuid& CustomerId = Pair.Key;
		FCustomerInstanceData& Customer = Pair.Value;
		
		SimulateHealth(Customer, NewTime);
	}
	LastUpdateHour = NewTime;
}

void UCustomerSimulationSubsystem::OnDayChanged(const FInGameTime& NewTime)
{
	// ToDo update subsets over one day
	for (TPair<FGuid, FCustomerInstanceData>& Pair : Customers)
	{
		FGuid& CustomerId = Pair.Key;
		FCustomerInstanceData& Customer = Pair.Value;
		
		UpdateCustomerAge(Customer, NewTime);
		UpdateCustomerFunds(Customer);
		SimulateCustomerInventory(Customer);
	}
	GenerateNewCustomer(NewTime, LastUpdateDay);
	LastUpdateDay = NewTime;
}

void UCustomerSimulationSubsystem::OnWeekChanged(const FInGameTime& NewTime)
{
	for (TPair<FGuid, FCustomerInstanceData>& Pair : Customers)
	{
		FGuid& CustomerId = Pair.Key;
		FCustomerInstanceData& Customer = Pair.Value;
		
		SimulateCustomerRelationships(Customer);
		UpdateCustomerPreferences(Customer);
	}
	LastUpdateWeek = NewTime;
}

void UCustomerSimulationSubsystem::OnMonthChanged(const FInGameTime& NewTime)
{
	for (TPair<FGuid, FCustomerInstanceData>& Pair : Customers)
	{
		FGuid& CustomerId = Pair.Key;
		FCustomerInstanceData& Customer = Pair.Value;
		
		SimulateProfessionChanges(Customer);
	}
	LastUpdateMonth = NewTime;
}

void UCustomerSimulationSubsystem::UpdateCustomerAge(FCustomerInstanceData& Customer, FInGameTime NewTime)
{
	// Called once per day
	const int32 PreviousWholeAge = FMath::FloorToInt(Customer.Age);
	
	Customer.Age += 1.f/365.f; // ToDo Use FInGameTime::DifferenceInDays
	
	const int32 NewWholeAge = FMath::FloorToInt(Customer.Age);
	
	// Check if birthday occurred
	if (NewWholeAge > PreviousWholeAge)
	{
		if (LogSubsystem)
		{
			LogSubsystem->LogCustomerBirthday(Customer);
		}
	}
}

void UCustomerSimulationSubsystem::UpdateCustomerFunds(FCustomerInstanceData& Customer)
{
	
}

void UCustomerSimulationSubsystem::SimulateCustomerInventory(FCustomerInstanceData& Customer)
{
	
}

void UCustomerSimulationSubsystem::SimulateHealth(FCustomerInstanceData& Customer, FInGameTime NewTime)
{
	//Health, health conditions, death
}

void UCustomerSimulationSubsystem::UpdateCustomerPreferences(FCustomerInstanceData& Customer)
{
	//ToDo develop/lose preferences based on what the customer has recently tried
	// Chance to lose or add preferences
	constexpr float RemoveChance = 0.50f;
	constexpr float AddChance = 0.05f; 

	if (Customer.Preferences.PreferredTags.Num() > 0 && FMath::FRand() < RemoveChance)
	{
		TArray<FGameplayTag> CurrentTags;
		Customer.Preferences.PreferredTags.GetGameplayTagArray(CurrentTags);
		const int32 RemoveIndex = FMath::RandRange(0, CurrentTags.Num() - 1);
		Customer.Preferences.PreferredTags.RemoveTag(CurrentTags[RemoveIndex]);
		if (LogSubsystem)
		{
			LogSubsystem->LogCustomerLostPreference(Customer, CurrentTags[RemoveIndex]);
		}
		return;
	}
	if (FMath::FRand() < AddChance)
	{
		TArray<FGameplayTag> AddedTags;
		AddedTags = GenerateNewCustomerPreferences(Customer, 1);
		if (LogSubsystem)
		{
			LogSubsystem->LogCustomerLostPreference(Customer, AddedTags[0]);
		}
	}
}

void UCustomerSimulationSubsystem::SimulateProfessionChanges(FCustomerInstanceData& Customer)
{
	//Chance to level up profession or lose job
	//If no job, try to find one (low probability to SelectNewCustomerProfession)
}

//Simulates progression of all relationships of one customer
void UCustomerSimulationSubsystem::SimulateCustomerRelationships(FCustomerInstanceData& Customer)
{
	for (TPair<FGuid, TSharedPtr<FCustomerRelationship>>& Pair : Customer.Relationships)
	{
		FGuid& OtherCustomerId = Pair.Key;
		const TSharedPtr<FCustomerRelationship> Rel = Pair.Value;
		
		SimulateRelationship(Customer, *Customers.Find(OtherCustomerId), Rel);
		// ToDo probability of adding a new relationship with existing customer
		// ToDo Probability of interaction based on relationship type
	}
}

//Simulates progression of the relationship between two customers (can be used for customer interactions e.g. in Tavern)
void UCustomerSimulationSubsystem::SimulateRelationship(FCustomerInstanceData& CustomerA, FCustomerInstanceData& CustomerB, TSharedPtr<FCustomerRelationship> Rel)
{
	FCustomerRelationship* RelationshipPtr;
	if (!Rel)
	{
		RelationshipPtr = GetOrCreateRelationship(CustomerA, CustomerB);
	}
	else
	{
		RelationshipPtr = Rel.Get();
	}
	if (!RelationshipPtr)
	{
		return;
	}
	
	// Random value for interaction outcome weighted by relationship strength:
	float StrengthMultiplier = FMath::GetMappedRangeValueClamped(FVector2D(-100.f, 100.f), FVector2D(0.5f, 1.5f), RelationshipPtr->Strength);
	float BaseRandom = FMath::FRandRange(-5.f, 5.f);
	float InteractionOutcome = BaseRandom * StrengthMultiplier;
	
	const FRelationshipTransitionRule* Rule = RelationshipPtr->AdvanceRelationship(InteractionOutcome, RulesByType);
	
	if (Rule && LogSubsystem)
	{
		LogSubsystem->LogCustomerRelationshipUpdate(CustomerA, CustomerB, *Rule);
	}
	// ToDo Take result from Relationship-Update, check if nullptr, if not, propagate result to other customer
	// ToDo Update household if true (add or remove member)
}

void UCustomerSimulationSubsystem::CreateCustomerInstance()
{
	FCustomerInstanceData NewCustomer;
	NewCustomer.InitializeFromDataAsset();
	
	//ToDo random starter relationships with existing customers, preferably with customers with same last time -> family
	//ToDo create household (new of add to existing household of relationship), create GUId for household if new
	
	//Random Profession selected from ProfessionDataAssets:
	
	SelectNewCustomerProfession(NewCustomer);
	
	//Random preferences:
	
	constexpr int32 MaxNumberPreferenceTags = 10;
	int32 NumberOfTagsToAdd = FMath::RandRange(0, MaxNumberPreferenceTags);
	GenerateNewCustomerPreferences(NewCustomer, NumberOfTagsToAdd);
	
	//ToDo random starting health conditions (low chance)
	
	Customers.Add(NewCustomer.CustomerId, NewCustomer);
	
	// Log creation of new customer in-game:
	if (LogSubsystem)
	{
		LogSubsystem->LogNewCustomerCreatedUpdate(NewCustomer);
	}
}

void UCustomerSimulationSubsystem::SelectNewCustomerProfession(FCustomerInstanceData& Customer)
{
	if (ProfessionDataAssets.Num() > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, ProfessionDataAssets.Num() - 1);

		TSoftObjectPtr<UProfessionDataAsset> SelectedProfessionSoft = ProfessionDataAssets[RandomIndex];
		UProfessionDataAsset* SelectedProfession = SelectedProfessionSoft.LoadSynchronous();

		if (SelectedProfession)
		{
			Customer.Profession.Definition = SelectedProfession;
			Customer.Profession.Level = 1;

			// Randomize starting level:
			Customer.Profession.Level = FMath::RandRange(1, 3);
			
			//ToDo random funds based on profession income and add to household
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load profession at index %d"), RandomIndex);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateCustomerInstance: No ProfessionDataAssets found."));
	}
}

TArray<FGameplayTag> UCustomerSimulationSubsystem::GenerateNewCustomerPreferences(FCustomerInstanceData& Customer, int32 NumberOfTagsToAdd)
{
	TArray<FGameplayTag> AddedTags;
	
	FGameplayTagContainer PurchasableChildren;

	PurchasableChildren = UGameplayTagsManager::Get().RequestGameplayTagChildren(MPGameplayTags::TAG_Purchasable);

	if (PurchasableChildren.Num() > 0)
	{
		TArray<FGameplayTag> TagArray;
		PurchasableChildren.GetGameplayTagArray(TagArray);
		
		NumberOfTagsToAdd = FMath::Clamp(NumberOfTagsToAdd, 0, PurchasableChildren.Num());
		
		for (int32 i = 0; i < NumberOfTagsToAdd; i++)
		{
			// ToDo weigh tags during selection based on funds
			// Filter out already-owned preferences:
			TagArray.RemoveAll([&Customer](const FGameplayTag& Tag)
			{
				return Customer.Preferences.PreferredTags.HasTagExact(Tag);
			});

			if (TagArray.Num() == 0)
			{
				return AddedTags; // No new preferences left to add
			}
			
			const int32 RandomIndex = FMath::RandRange(0, TagArray.Num() - 1);
			const FGameplayTag RandomPreference = TagArray[RandomIndex];
			AddedTags.Add(RandomPreference);
			Customer.Preferences.PreferredTags.AddTag(RandomPreference);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No child tags found under TAG_Purchasable."));
	}
	return AddedTags;
}

void UCustomerSimulationSubsystem::GenerateChildCustomer(FCustomerInstanceData& ParentCustomer)
{
	//Add parent & child relationships
	//Add sibling relationships
	//Add extended family relationships
	//Add to parent household
	//Log new customer born
}

void UCustomerSimulationSubsystem::GenerateNewCustomer(const FInGameTime& NewTime, FInGameTime& LastUpdate)
{
	//Don't start generating until LastUpdateTime is properly initialized
	if (!bHasInitializedTime)
	{
		bHasInitializedTime = true;
		return;
	}
	
	//Calculate chance of creating a new customer based on number of existing customers
	const int32 ExistingCustomerCount = Customers.Num();
	
	//UE_LOG(LogTemp, Warning, TEXT("Number of Customers: %d"), ExistingCustomerCount);
	
	if (ExistingCustomerCount >= MaximumCustomerCount)
	{
		return;
	}
	
	//Increase BaseSpawChance according to time passed since last update:
	const float DeltaMinutes = FInGameTime::DifferenceInMinutes(NewTime, LastUpdate);
	const float DeltaHours = DeltaMinutes / 60.f;

	const float GrowthMultiplier = 1.f / (1.f + ExistingCustomerCount * DampingFactor);

	// Growth always positive, but slower with more customers
	BaseSpawnChance += BaseSpawnChanceIncrease * GrowthMultiplier;

	BaseSpawnChance = FMath::Clamp(BaseSpawnChance, 0.f, 1.f);
	
	if (FMath::FRand() < BaseSpawnChance)
	{
		CreateCustomerInstance();
		BaseSpawnChance = 0.f;
		//LogAllCustomerData();
	}
	//UE_LOG(LogTemp, Warning, TEXT("New Spawn Chance: %f"), BaseSpawnChance);
}

void UCustomerSimulationSubsystem::ScheduleVisit(FCustomerInstanceData& Customer)
{
}

void UCustomerSimulationSubsystem::BuildTransitionMap()
{
	if (!RelationshipRulesAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("RelationshipRulesAsset not assigned!"));
		return;
	}
	
	TArray<FRelationshipTransitionRule> TransitionRules = RelationshipRulesAsset->TransitionRules;
	
	RulesByType.Empty();

	for (const FRelationshipTransitionRule& Rule : TransitionRules)
	{
		RulesByType.FindOrAdd(Rule.From).Add(Rule);
		
		//Log added Relationship Transitions:
		const UEnum* EnumPtr = StaticEnum<ERelationshipTypes>();
		if (EnumPtr)
		{
			const FString FromDisplayName = EnumPtr->GetDisplayNameTextByValue((int64)Rule.From).ToString();
			const FString ToDisplayName   = EnumPtr->GetDisplayNameTextByValue((int64)Rule.To).ToString();

			UE_LOG(LogTemp, Log, TEXT("Relationship Rule: From %s -> To %s"), *FromDisplayName, *ToDisplayName);
		}
	}
}

FHousehold* UCustomerSimulationSubsystem::GetHouseholdById(const FGuid& Id)
{
	return Households.Find(Id);
}

void UCustomerSimulationSubsystem::ModifyHouseholdFunds(const FGuid* Id, float FundsToAdd)
{
	GetHouseholdById(*Id)->SharedFunds += FundsToAdd;
}

float UCustomerSimulationSubsystem::GetHouseholdFunds(const FGuid* Id)
{
	return GetHouseholdById(*Id)->SharedFunds;
}

FCustomerRelationship* UCustomerSimulationSubsystem::GetOrCreateRelationship(FCustomerInstanceData& CustomerA, FCustomerInstanceData& CustomerB)
{
	
	if (&CustomerA == &CustomerB)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't create relationship with self."));
		return nullptr;
	}
	
	TSharedPtr<FCustomerRelationship>* ExistingRelA = CustomerA.Relationships.Find(CustomerB.CustomerId);
	if (ExistingRelA && ExistingRelA->IsValid())
	{
		return ExistingRelA->Get();
	}
	
	//Create new relationship:
	TSharedPtr<FCustomerRelationship> NewRel = MakeShared<FCustomerRelationship>();
	NewRel->CustomerAId = CustomerA.CustomerId;
	NewRel->CustomerBId = CustomerB.CustomerId;
	NewRel->Type = ERelationshipTypes::Acquaintance;
	NewRel->Strength = 50.f;

	// Add to both customers:
	CustomerA.Relationships.Add(CustomerB.CustomerId, NewRel);
	CustomerB.Relationships.Add(CustomerA.CustomerId, NewRel);

	return NewRel.Get();
}

void UCustomerSimulationSubsystem::LogAllCustomerData() const
{
	UE_LOG(LogTemp, Log, TEXT("=== CustomerSimulationSubsystem: Customer Data ==="));
	UE_LOG(LogTemp, Log, TEXT("Spawn Chance: %f"), BaseSpawnChance);
	
	int32 Index = 1;
	for (const TPair<FGuid, FCustomerInstanceData>& Pair : Customers)
	{
		const FGuid& CustomerId = Pair.Key;
		const FCustomerInstanceData& Customer = Pair.Value;

		UE_LOG(LogTemp, Log, TEXT("Customer %d: (ID: %s)"), Index, *CustomerId.ToString());
		UE_LOG(LogTemp, Log, TEXT("   Name: %s"), *Customer.CustomerName.ToString());
		UE_LOG(LogTemp, Log, TEXT("   Goodwill: %.2f"), Customer.Goodwill);
		UE_LOG(LogTemp, Log, TEXT("   VisitCount: %d"), Customer.VisitCount);

		Index++;
	}
}
