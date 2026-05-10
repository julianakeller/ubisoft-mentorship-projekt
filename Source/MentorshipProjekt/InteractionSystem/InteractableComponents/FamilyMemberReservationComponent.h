// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponentBase.h"
#include "ReservationComponentBase.h"
#include "MentorshipProjekt/NPCs/NPCCharacter.h"
#include "FamilyMemberReservationComponent.generated.h"

class APlayableFamilyMemberCharacter;
/**
 * 
 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UFamilyMemberReservationComponent : public UReservationComponentBase
{
	GENERATED_BODY()
	
public:

	virtual void OnInteract(AActor* Interactor) override;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TObjectPtr<UArrowComponent> FamilyMemberInteractionPoint;
};
