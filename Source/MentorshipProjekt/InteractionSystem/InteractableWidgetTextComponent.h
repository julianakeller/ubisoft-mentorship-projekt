// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/InputSettings.h"
#include "InteractableComponents/InteractableComponentBase.h"
#include "InteractableWidgetTextComponent.generated.h"

class UInteractionWidget;
/**
 * 
 */
class UMenuManager;

USTRUCT(BlueprintType)
struct FInteractionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AvailableText = FText::FromString("Interact");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractingText = FText::FromString("Interact");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKey InputKey;
	
	UPROPERTY(EditDefaultsOnly, Category="Parameters")
	//TMap<FHardwareDeviceIdentifier, UTexture2D*> DeviceNameToTexture;
	UTexture2D* ButtonTexture;
};

UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class MENTORSHIPPROJEKT_API UInteractableWidgetTextComponent : public UInteractableComponentBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	
	UPROPERTY(EditAnywhere, Category="Interaction")
	TArray<FInteractionEntry> InteractionEntries;
	
	virtual void OnRangeEntered(AActor* Interactor) override;
	virtual void OnRangeExited(AActor* Interactor) override;
	virtual void OnInteract(AActor* Interactor) override;
	
	UPROPERTY()
	UInteractionWidget* InteractionWidgetReference = nullptr;
	
private:
	
	void UpdateWidgetVisibility();
	void UpdateText();
};
