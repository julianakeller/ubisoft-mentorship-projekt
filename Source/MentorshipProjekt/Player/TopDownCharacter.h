// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MentorshipProjekt/NPCs/FamilyMember/PlayableFamilyMemberCharacter.h"
#include "GameFramework/Character.h"
#include "MentorshipProjekt/NPCs/Skills/SkillDataAsset.h"
#include "MentorshipProjekt/NPCs/Skills/SkillProgress.h"
#include "TopDownCharacter.generated.h"

class UCircularProgressWidget;
class UWidgetComponent;

UCLASS()
class MENTORSHIPPROJEKT_API ATopDownCharacter : public APlayableFamilyMemberCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATopDownCharacter();
	
	//Maps SkillNames to the skill's progress
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<FGameplayTag, FSkillProgress> AcquiredSkills;
	// ToDo is this the best way? Maybe player should also have instance data

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	
	void Move(const FInputActionValue& Input);
	
	void Look(const FInputActionValue& Input);
	
	void Zoom(const FInputActionValue& Input);
	
	void Interact();
	
	void InteractSecondary();
	
	void CloseMenu();

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	UInputMappingContext* IMC_Player;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	UInputAction* IA_MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	UInputAction* IA_LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	UInputAction* IA_ZoomAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	UInputAction* IA_Interact;
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	UInputAction* IA_InteractSecondary;
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	UInputAction* IA_CloseMenu;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float ZoomSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float MinZoom = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float MaxZoom = 1200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float TargetArmLength;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float ZoomInterpolationSpeed = 10.f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitializeSkills();
	
	void BeginWorkstationLock(AActor* Workstation);
	void EndWorkstationLock();
	
private:
	
	bool bIsLockedToWorkstation = false;
	TWeakObjectPtr<AActor> PendingWorkstation;

	float RotationAcceptanceDot = 0.985f; // 10 degrees tolerance when facing workstation

	void TickWorkstationRotation(float DeltaTime);
};
