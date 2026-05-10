// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomerInformationDisplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UCustomerInformationDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CustomerName;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Funds;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Age;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Occupation;
	
};
