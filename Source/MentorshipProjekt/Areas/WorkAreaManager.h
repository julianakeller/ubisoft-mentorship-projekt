// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorkAreaManager.generated.h"

class AWorkAreaBase;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API UWorkAreaManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	void RegisterWorkArea(AWorkAreaBase* Area);
	void UnregisterWorkArea(AWorkAreaBase* Area);

	const TArray<AWorkAreaBase*>& GetAllWorkAreas() const;

private:

	UPROPERTY()
	TArray<AWorkAreaBase*> WorkAreas;
};
