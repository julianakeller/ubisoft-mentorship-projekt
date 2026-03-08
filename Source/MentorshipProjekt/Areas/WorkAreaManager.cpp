// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkAreaManager.h"

void UWorkAreaManager::RegisterWorkArea(AWorkAreaBase* Area)
{
	if (Area)
	{
		WorkAreas.AddUnique(Area);
	}
}

void UWorkAreaManager::UnregisterWorkArea(AWorkAreaBase* Area)
{
	WorkAreas.Remove(Area);
}

const TArray<AWorkAreaBase*>& UWorkAreaManager::GetAllWorkAreas() const
{
	return WorkAreas;
}