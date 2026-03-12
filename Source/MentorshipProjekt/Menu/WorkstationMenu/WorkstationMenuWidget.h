// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorkstationMenuRowWidget.h"
#include "Components/VerticalBox.h"
#include "MentorshipProjekt/Menu/BaseMenuWidget.h"
#include "WorkstationMenuWidget.generated.h"

class UInteractableWorkstationComponent;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UWorkstationMenuWidget : public UBaseMenuWidget
{
	GENERATED_BODY()
	
public:

	virtual void InitializeMenu(AInteractableBase* Source) override;

	virtual void Open() override;
	
	virtual void Close() override;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MenuHeader;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox;
	
protected:

	UPROPERTY(BlueprintReadOnly)
	UInteractableWorkstationComponent* WorkstationComponent;
	
	void PopulateRows();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Workstation")
	TSubclassOf<UWorkstationMenuRowWidget> RowWidgetClass;
};
