#pragma once

#include "CoreMinimal.h"
#include "GameTimeSubsystem.h"
#include "LogMessage.generated.h"

UENUM(BlueprintType)
enum class ELogMessageType : uint8
{
	Default UMETA(DisplayName="Default"),
	Warning UMETA(DisplayName="Warning"),
	Important UMETA(DisplayName="Important")
};

USTRUCT(BlueprintType)
struct FLogMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FText Message;

	UPROPERTY(BlueprintReadOnly)
	ELogMessageType Type;

	UPROPERTY(BlueprintReadOnly)
	FInGameTime Timestamp;
	
	UPROPERTY(BlueprintReadOnly)
	float Lifetime = 10.f;

	FLogMessage()
	{
		Type=ELogMessageType::Default;
	}

	FLogMessage(const FText& InMessage, ELogMessageType InType, FInGameTime CreationTime) : Message(InMessage), Type(InType), Timestamp(CreationTime)
	{
		
	}
};