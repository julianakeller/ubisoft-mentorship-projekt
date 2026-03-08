// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WorkAreaBase.generated.h"

class AInteractableBase;
class AInteractableWorkStation;

UCLASS()
class MENTORSHIPPROJEKT_API AWorkAreaBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorkAreaBase();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Work Area")
	FName WorkAreaName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work Area")
	TArray<AInteractableBase*> WorkStations; //ToDo should be limited to interactables with workstation component only

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Work Area")
	USceneComponent* VisitorInteractablesRoot;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void OnConstruction(const FTransform& Transform) override;

	//Container for workstations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Work Area")
	USceneComponent* WorkStationsRoot;

	void GatherWorkStations();
	
public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visit Area")
	class UVisitAreaComponent* VisitAreaComponent; //Optional, assign if area is visitable by customers
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	UBoxComponent* DebugBounds;
	
private:
	void UpdateDebugBounds() const;
};
