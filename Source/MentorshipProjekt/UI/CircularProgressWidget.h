// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CircularProgressWidget.generated.h"

/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UCircularProgressWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetProgress(float Value);
	
	void EnableWidget();
	
	void DisableWidget();

protected:
	UPROPERTY(meta=(BindWidget))
	class UImage* ProgressImage;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* MID;
};
