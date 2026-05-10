// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponentBase.generated.h"


class UCustomerReservationComponent;
class UGameTimeSubsystem;
class UPurchasableManagerSubsystem;
class UWSInteractionComponentBase;
class IWorkstationInteractionInterface;
class UFamilyMemberReservationComponent;
class UInteractableWorkstationComponent;
class USingleStageProductionComponent;
class AInteractableBase;

UCLASS(Abstract, ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent) )
class MENTORSHIPPROJEKT_API UInteractableComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractableComponentBase();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	
	UPROPERTY()
	AInteractableBase* CachedInteractable = nullptr;

	// Implemented by child component classes:
	virtual void OnInteract(AActor* Interactor) {}
	virtual void OnInteractSecondary(AActor* Interactor) {}
	virtual void OnRangeEntered(AActor* Interactor) {}
	virtual void OnRangeExited(AActor* Interactor) {}

private:	
	
	//Event subscribers:
	UFUNCTION()
	void HandleInteract(AActor* Interactor);
	
	UFUNCTION()
	void HandleInteractSecondary(AActor* Interactor);

	UFUNCTION()
	void HandleRangeEntered(AActor* Interactor);

	UFUNCTION()
	void HandleRangeExited(AActor* Interactor);
		
public:
	
	UPROPERTY()
	UWSInteractionComponentBase* ProductionComponent = nullptr;
	
	UPROPERTY()
	UInteractableWorkstationComponent* ProductionWorkstation = nullptr;
	
	UPROPERTY()
	UFamilyMemberReservationComponent* FamilyMemberReservationComponent = nullptr;
	
	UPROPERTY()
	UCustomerReservationComponent* CustomerReservationComponent = nullptr;
	
	UPROPERTY()
	UGameTimeSubsystem* GameTimeSubsystem;

	UPROPERTY()
	UPurchasableManagerSubsystem* PurchasableManager;
};
