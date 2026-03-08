// Fill out your copyright notice in the Description page of Project Settings.


#include "FamilyMemberDataAsset.h"

const FWorkPreference* UFamilyMemberDataAsset::GetWorkPreference(TSubclassOf<AWorkAreaBase> WorkAreaClass) const
{
	for (const FWorkPreference& Pref : WorkPreferences)
	{
		if (Pref.PreferredWorkAreaClass == WorkAreaClass)
		{
			return &Pref;
		}
	}

	return nullptr;
}