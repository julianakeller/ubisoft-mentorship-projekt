// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorkAreaDropdownWidget.generated.h"

class UShiftManager;
class UWorkAreaDropdownOptionWidget;
class UVerticalBox;
class UShiftBlockWidget;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorkAreaChosen, FName, SelectedArea);

UCLASS()
class MENTORSHIPPROJEKT_API UWorkAreaDropdownWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeDropdown(UShiftBlockWidget* InOwner);
	
	UPROPERTY(BlueprintAssignable)
	FOnWorkAreaChosen OnWorkAreaChosen;
	
	UFUNCTION()
	void ToggleDropdown();
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* OptionsBox;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWorkAreaDropdownOptionWidget> OptionWidgetClass;
	
private:
	UPROPERTY()
	UShiftBlockWidget* OwnerShift;
	
	bool bIsOpen = false;

	UFUNCTION()
	void HandleOptionSelected(FName SelectedID);
	
	//UShiftManager* ShiftManager = nullptr;
};
