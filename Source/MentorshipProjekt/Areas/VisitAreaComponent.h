// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VisitAreaComponent.generated.h"


class AInteractableBase;
class AInteractableNPC;

UCLASS( ClassGroup=(Area), meta=(BlueprintSpawnableComponent) )
class MENTORSHIPPROJEKT_API UVisitAreaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVisitAreaComponent();
	
	UFUNCTION(BlueprintCallable)
	TArray<AInteractableBase*> GetFreeInteractables() const; //ToDo should be somehow limited to visitor interactables only
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	USceneComponent* VisitorInteractablesRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visit Area")
	TArray<AInteractableBase*> VisitorInteractables; //ToDo should be somehow limited to visitor interactables only

	void GatherVisitorInteractables();
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
