// Fill out your copyright notice in the Description page of Project Settings.


#include "LogSubsystem.h"
#include "GameplayTagsManager.h"
#include "MentorshipProjekt/NPCs/FamilyInstanceData.h"
#include "MentorshipProjekt/NPCs/Customer/CustomerInstanceData.h"

void ULogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency(UGameTimeSubsystem::StaticClass());

	Super::Initialize(Collection);
	
	GameTimeSubsystem = GetWorld()->GetSubsystem<UGameTimeSubsystem>();
}

void ULogSubsystem::AddLogMessage(const FText& Message, ELogMessageType Type)
{
	if (!GameTimeSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("GameTimeSubsystem not found."));
		return;
	}

	FLogMessage NewMessage(Message, Type, GameTimeSubsystem->GetCurrentTime());

	MessageHistory.Add(NewMessage);

	CleanupOldMessages();

	OnLogMessageAdded.Broadcast(NewMessage);
	
	//UE_LOG(LogTemp, Warning, TEXT("Broadcasted OnLogMessageAdded."));
}

void ULogSubsystem::LogDefault(const FText& Message)
{
	AddLogMessage(Message, ELogMessageType::Default);
}

void ULogSubsystem::LogWarning(const FText& Message)
{
	AddLogMessage(Message, ELogMessageType::Warning);
}

void ULogSubsystem::LogImportant(const FText& Message)
{
	AddLogMessage(Message, ELogMessageType::Important);
}

TArray<FLogMessage> ULogSubsystem::GetRecentMessages() const
{
	return MessageHistory;
}

void ULogSubsystem::CleanupOldMessages()
{
	MessageHistory.RemoveAll([&](const FLogMessage& Msg){return FInGameTime::DifferenceInMinutes(
	Msg.Timestamp,
	GameTimeSubsystem->GetCurrentTime()) > StoredHistoryDuration;});
}

#pragma region Custom Logging Messages Functions

void ULogSubsystem::LogNewCustomerCreatedUpdate(const FCustomerInstanceData& Customer)
{
	FText NewCustomerName = FText::FromName(Customer.CustomerName);
	int32 Age = Customer.Age;
	FCustomerProfession Profession = Customer.Profession;
	FText ProfessionName = FText::FromString(TEXT("Unemployed"));

	if (Profession.Definition)
	{
		ProfessionName = FText::FromName(Profession.Definition->ProfessionName);
	}
	
	FText Message = FText::Format(
		NSLOCTEXT("Log", "RelationshipUpdate", "A new {0} named {1}, aged {2}, has moved to town."),
		ProfessionName,
		NewCustomerName,
		Age);
	
	LogDefault(Message);
}

void ULogSubsystem::LogCustomerRelationshipUpdate(const FCustomerInstanceData& CustomerA, const FCustomerInstanceData& CustomerB, const FRelationshipTransitionRule& Rule)
{
	FText Message;
	
	FText NameA = FText::FromName(CustomerA.CustomerName);
	FText NameB = FText::FromName(CustomerB.CustomerName);

	FText RelationshipText = FText::FromName(TEXT("Unknown"));
	
	//StaticEnum returns pointer to UEnum object of ERelationshipTypes at runtime
	if (const UEnum* RelTypesPtr = StaticEnum<ERelationshipTypes>())
	{
		//GetDisplayName gets the name defined with UMETA(DisplayName...)
		//UENUM stores values as integers -> static_cast<int64>
		RelationshipText = RelTypesPtr->GetDisplayNameTextByValue(static_cast<int64>(Rule.To));
	}

	if (RelationshipText.EqualTo(FText::FromName(TEXT("Unknown"))))
	{
		Message = FText::Format(NSLOCTEXT("Log", "RelationshipUpdate", "{0} and {1}'s relationship has changed."),
		NameA,
		NameB);
	}
		Message = FText::Format(
		NSLOCTEXT("Log", "RelationshipUpdate", "{0} has become {1}'s {2}"),
		NameA,
		NameB,
		RelationshipText
	);
	
	// Message = CustomerAName has become CustomerBName's NewRelationshipType
	LogDefault(Message);
}

void ULogSubsystem::LogCustomerBirthday(const FCustomerInstanceData& Customer)
{
	FText Message;

	const FText CustomerName = FText::FromName(Customer.CustomerName);

	const int32 NewAge = FMath::FloorToInt(Customer.Age);

	Message = FText::Format(
		NSLOCTEXT("Log", "CustomerBirthday", "{0} has turned {1}."),
		CustomerName,
		FText::AsNumber(NewAge)
	);

	LogDefault(Message);
}

void ULogSubsystem::LogCustomerNewPreference(const FCustomerInstanceData& Customer, FGameplayTag NewPreference)
{
	
	const FText CustomerName = FText::FromName(Customer.CustomerName);

	FText PreferenceText = FText::FromString(NewPreference.GetTagName().ToString());

	// ToDo If tag has display name in tag table, use that instead

	const FText Message = FText::Format(
		NSLOCTEXT("Log", "CustomerNewPreference", "{0} has developed a preference for {1}."),
		CustomerName,
		PreferenceText
	);

	LogDefault(Message);
}

void ULogSubsystem::LogCustomerLostPreference(const FCustomerInstanceData& Customer, FGameplayTag LostPreference)
{
	const FText CustomerName = FText::FromName(Customer.CustomerName);

	FText PreferenceText = FText::FromString(LostPreference.GetTagName().ToString());

	const FText Message = FText::Format(
		NSLOCTEXT("Log", "CustomerLostPreference", "{0} is no longer interested in {1}."),
		CustomerName,
		PreferenceText
	);

	LogDefault(Message);
}

void ULogSubsystem::LogFamilyMemberHighHappiness(const FFamilyInstanceData& FamilyMember)
{
	const FText Name = FText::FromName(FamilyMember.FamilyMemberData->FamilyMemberName);

	const FText Message = FText::Format(
		NSLOCTEXT("Log", "HighHappiness", "{0} is feeling extremely happy."),
		Name
	);

	LogDefault(Message);
}

void ULogSubsystem::LogFamilyMemberLowHappiness(const FFamilyInstanceData& Member)
{
	const FText Name = FText::FromName(Member.FamilyMemberData->FamilyMemberName);

	const FText Message = FText::Format(
		NSLOCTEXT("Log", "LowHappiness", "{0} is feeling extremely unhappy and may become disloyal."),
		Name
	);

	LogWarning(Message);
}

void ULogSubsystem::LogFamilyMemberHighLoyalty(const FFamilyInstanceData& Member)
{
	const FText Name = FText::FromName(Member.FamilyMemberData->FamilyMemberName);

	const FText Message = FText::Format(
		NSLOCTEXT("Log", "HighLoyalty", "{0} is now extremely loyal to the family."),
		Name
	);

	LogImportant(Message);
}

void ULogSubsystem::LogFamilyMemberLowLoyalty(const FFamilyInstanceData& Member)
{
	const FText Name = FText::FromName(Member.FamilyMemberData->FamilyMemberName);

	const FText Message = FText::Format(
		NSLOCTEXT("Log", "LowLoyalty", "{0}'s loyalty to the family has become extremely low."),
		Name
	);

	LogWarning(Message);
}

#pragma endregion 