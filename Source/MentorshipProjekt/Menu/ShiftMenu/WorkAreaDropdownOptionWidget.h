// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorkAreaDropdownOptionWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorkAreaSelected, FName, WorkAreaID);

UCLASS()
class MENTORSHIPPROJEKT_API UWorkAreaDropdownOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnWorkAreaSelected OnSelected;
	
	void InitializeOption(FName InWorkAreaID, const FText& InDisplayName);

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OptionText;

private:

	FName WorkAreaID;

	UFUNCTION()
	void HandleClicked();
};
