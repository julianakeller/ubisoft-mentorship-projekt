// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "WorkAreaButtonWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorkAreaClicked, FName, AreaName);

UCLASS()
class MENTORSHIPPROJEKT_API UWorkAreaButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeButton(FName InAreaName, const FLinearColor& InColor);
	
	UPROPERTY(BlueprintAssignable)
	FOnWorkAreaClicked OnClicked;
	
	UPROPERTY(meta=(BindWidget))
	UBorder* BackgroundBorder;
	
	UPROPERTY(meta=(BindWidget))
	UImage* SelectionBorder;
	
	void SetSelected(bool bSelected);
	
	FName GetAreaName();
	
protected:
	
	void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UButton* Button;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ButtonText;

	FName AreaName;
	
	UPROPERTY()
	FLinearColor AreaColor;
	
	UFUNCTION()
	void HandleClicked();
	
	void UpdateVisual();
};
