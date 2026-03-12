// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorkstationMenuRowWidget.generated.h"

class UPurchasableDefinition;
class UInteractableWorkstationComponent;
class UButton;
class UTextBlock;
class USpinBox;
class UImage;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UWorkstationMenuRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void InitializeRow(UInteractableWorkstationComponent* InComponent, UPurchasableDefinition* InPurchasable, int32 Index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* CheckedImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* UncheckedImage;
	
protected:
	
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UButton* ButtonEnable;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* PurchasableName;

	UPROPERTY(meta=(BindWidget))
	USpinBox* TargetAmount;
	
	UPROPERTY(meta=(BindWidget))
	USpinBox* TargetPriority;

	UPROPERTY(meta=(BindWidget))
	UImage* Checkmark;

	UPROPERTY()
	UInteractableWorkstationComponent* WorkstationComponent;

	UPROPERTY()
	UPurchasableDefinition* Purchasable;

	int32 SettingIndex =  INDEX_NONE;
	
	UFUNCTION()
	void OnEnableButtonClicked();

	UFUNCTION()
	void OnAmountValueChanged(float Value);
	
	UFUNCTION()
	void OnPriorityValueChanged(float Value);

	void RefreshVisuals();
};
