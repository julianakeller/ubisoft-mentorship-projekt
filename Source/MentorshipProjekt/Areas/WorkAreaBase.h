// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WorkAreaBase.generated.h"

class UTextRenderComponent;
class AInteractableBase;
class AInteractableWorkStation;

UCLASS()
class MENTORSHIPPROJEKT_API AWorkAreaBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorkAreaBase();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditAnywhere, Category = "Work Area")
	FName WorkAreaName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Work Area")
	TArray<AInteractableBase*> WorkStations; //ToDo should be limited to interactables with workstation component only

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Work Area")
	TArray<AInteractableBase*> CustomerWorkstations;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Work Area")
	USceneComponent* VisitorInteractablesRoot;
	
	virtual void Tick(float DeltaTime) override;

	//Optional, assign if area is visitable by customers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visit Area")
	class UVisitAreaComponent* VisitAreaComponent; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Work Area")
	FLinearColor WorkAreaColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Work Area")
	UBoxComponent* WorkAreaExtents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Work Area")
	UTextRenderComponent* WorkAreaLabel;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	//Container for workstations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Work Area")
	USceneComponent* WorkStationsRoot;

	void GatherWorkStations();
};
