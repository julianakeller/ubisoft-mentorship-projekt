// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractablesInterface.h"
#include "InteractionWidget.h"
#include "InteractableBase.generated.h"

class USphereComponent;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EInteractionState : uint8
{
	Idle, //Out of range
	Available, //In Range
	Interacting, //Menu in use
	Disabled //not currently interactable
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractSignature, AActor*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractSecondarySignature, AActor*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRangeSignature, AActor*, Interactor);

UCLASS()
// InteractableBase implements player proximity detection, interaction widget display, generic interaction triggers
class MENTORSHIPPROJEKT_API AInteractableBase : public AActor, public IInteractablesInterface
{
	GENERATED_BODY()
	
public:	
	AInteractableBase();
	
	//Interactables Interface
	virtual void OnInteractionRangeEntered() override;
	virtual void OnInteractionRangeExited() override;
	virtual void OnInteracted(AActor* Character) override;
	virtual void OnInteractedSecondary(AActor* Character) override;
	virtual void HandleInteraction(AActor* Character) override;
	virtual void HandleInteractionSecondary(AActor* Character) override;
	virtual bool CanBeInteracted() const override;
	virtual void SetInteractionText(FText NewInteractionText) override;
	virtual FText GetInteractionText() const override {return InteractionText;};
	virtual void SetInteractionState(EInteractionState NewState);
	virtual EInteractionState GetInteractionState() {return InteractionState;};
	
public:
	
	//Delegates
	UPROPERTY(BlueprintAssignable)
	FOnInteractSignature OnInteractEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnInteractSignature OnInteractSecondaryEvent;

	UPROPERTY(BlueprintAssignable)
	FOnRangeSignature OnRangeEnteredEvent;

	UPROPERTY(BlueprintAssignable)
	FOnRangeSignature OnRangeExitedEvent;
	
protected:
	
	virtual void BeginPlay() override;
	
	void UpdateWidgetVisibility();
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* InteractionRange = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* InteractableMesh = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UWidgetComponent* InteractionWidgetComponent = nullptr;
	
	EInteractionState InteractionState = EInteractionState::Idle;

private:
	
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
	
	UPROPERTY(EditAnywhere, Category = "Parameters")
	FText InteractionText = FText::FromString("Interact");
	
	UPROPERTY()
	UInteractionWidget* InteractionWidgetReference = nullptr;
};
