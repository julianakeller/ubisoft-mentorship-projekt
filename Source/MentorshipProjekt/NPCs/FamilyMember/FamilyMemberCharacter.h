// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../NPCCharacter.h"
#include "PlayableFamilyMemberCharacter.h"
#include "GameFramework/Character.h"
#include "MentorshipProjekt/UI/FamilyMemberNameLabelWidget.h"
#include "FamilyMemberCharacter.generated.h"

struct FFamilyInstanceData;
class UFamilyMemberBrainComponent;

UCLASS()
class MENTORSHIPPROJEKT_API AFamilyMemberCharacter : public APlayableFamilyMemberCharacter
{
	GENERATED_BODY()

public:
	AFamilyMemberCharacter();
	
	void Initialize(FFamilyInstanceData* InData, FGuid InFamilyInstanceId);
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	//UFamilyMemberBrainComponent* FamilyBrainComponent; //Shift and work simulation

	virtual void Tick(float DeltaTime) override;

	FFamilyInstanceData GetInstanceData() const;

	UPROPERTY()
	FGuid FamilyMemberId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* NameLabelWidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFamilyMemberNameLabelWidget* NameLabelWidget;
	
protected:
	
	virtual void BeginPlay() override;
	
private:
	
	FName CachedName;
};
