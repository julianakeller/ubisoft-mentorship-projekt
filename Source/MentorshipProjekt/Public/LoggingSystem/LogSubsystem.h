// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LogMessage.h"
#include "LogSubsystem.generated.h"

/**
 * 
 */

struct FFamilyInstanceData;
struct FGameplayTag;
struct FRelationshipTransitionRule;
struct FCustomerInstanceData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLogMessageAdded, const FLogMessage&, Message);

UCLASS()
class MENTORSHIPPROJEKT_API ULogSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	void Initialize(FSubsystemCollectionBase& Collection);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Log")
	float MessageLifetimeSeconds = 5.f;
	
	//How old log messages can be before being discarded
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Log")
	float StoredHistoryDuration = 60.f;
	
	UPROPERTY(BlueprintAssignable)
	FOnLogMessageAdded OnLogMessageAdded;

	UFUNCTION(BlueprintCallable)
	void AddLogMessage(const FText& Message, ELogMessageType Type);

	UFUNCTION(BlueprintCallable)
	void LogDefault(const FText& Message);

	UFUNCTION(BlueprintCallable)
	void LogWarning(const FText& Message);

	UFUNCTION(BlueprintCallable)
	void LogImportant(const FText& Message);
	
	UFUNCTION(BlueprintCallable)
	TArray<FLogMessage> GetRecentMessages() const;
	
	// ----------- Custom Logging ----------------
	
	UFUNCTION(BlueprintCallable)
	void LogCustomerRelationshipUpdate(const FCustomerInstanceData& CustomerA, const FCustomerInstanceData& CustomerB, const FRelationshipTransitionRule& Rule);
	
	UFUNCTION(BlueprintCallable)
	void LogNewCustomerCreatedUpdate(const FCustomerInstanceData& Customer);
	
	UFUNCTION(BlueprintCallable)
	void LogCustomerBirthday(const FCustomerInstanceData& Customer);

	UFUNCTION(BlueprintCallable)
	void LogCustomerNewPreference(const FCustomerInstanceData& Customer, FGameplayTag NewPreference);
	
	UFUNCTION(BlueprintCallable)
	void LogCustomerLostPreference(const FCustomerInstanceData& Customer, FGameplayTag LostPreference);
	
	UFUNCTION(BlueprintCallable)
	void LogFamilyMemberHighHappiness(const FFamilyInstanceData& FamilyMember);
	
	UFUNCTION(BlueprintCallable)
	void LogFamilyMemberLowHappiness(const FFamilyInstanceData& FamilyMember);
	
	UFUNCTION(BlueprintCallable)
	void LogFamilyMemberHighLoyalty(const FFamilyInstanceData& FamilyMember);
	
	UFUNCTION(BlueprintCallable)
	void LogFamilyMemberLowLoyalty(const FFamilyInstanceData& FamilyMember);
	
private:

	UPROPERTY()
	TArray<FLogMessage> MessageHistory;
	
	UGameTimeSubsystem* GameTimeSubsystem;

	void CleanupOldMessages();
};
