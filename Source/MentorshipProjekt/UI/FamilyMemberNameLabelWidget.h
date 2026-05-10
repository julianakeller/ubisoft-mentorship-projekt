// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FamilyMemberNameLabelWidget.generated.h"

/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UFamilyMemberNameLabelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SetNameText(const FText& NewText);
	
private:
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* FamilyMemberName;
	
};
