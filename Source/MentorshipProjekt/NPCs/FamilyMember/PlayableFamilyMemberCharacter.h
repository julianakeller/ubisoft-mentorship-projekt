// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MentorshipProjekt/NPCs/NPCCharacter.h"
#include "PlayableFamilyMemberCharacter.generated.h"

class UCircularProgressWidget;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EActionType : uint8
{
	None,
	Cooking,
	PreparingFood,
	Bartending,
	Sowing,
	Harvesting,
	Watering,
	FillingBarrel,
	TendingCounter
};

UCLASS()
class MENTORSHIPPROJEKT_API APlayableFamilyMemberCharacter : public ANPCCharacter
{
	GENERATED_BODY()

public:
	
	APlayableFamilyMemberCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* ProgressWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> ProgressWidgetClass;
	
	virtual void BeginPlay() override;
	
public:	
	
	UPROPERTY(BlueprintReadOnly)
	EActionType CurrentAction;
	
	virtual void Tick(float DeltaTime) override;
	
	UCircularProgressWidget* GetProductionProgressWidget();
};
