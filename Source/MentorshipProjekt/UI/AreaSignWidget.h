// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "AreaSignWidget.generated.h"

/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UAreaSignWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	void SetName(FName AreaNameToSet);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work Area")
	FName NameToSet;
	
	virtual void NativeConstruct() override;;
	
protected:
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* AreaName;
};
