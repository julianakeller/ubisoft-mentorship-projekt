// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkAreaManager.h"
#include "WorkAreaBase.h"
#include "MentorshipProjekt/Tags/MPGameplayTags.h"
#include "MentorshipProjekt/InteractionSystem/InteractableComponents/InteractableWorkstationComponent.h"
#include "MentorshipProjekt/InteractionSystem/InteractableBase.h"

void UWorkAreaManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UWorkAreaManager::RegisterWorkArea(AWorkAreaBase* Area)
{
	UE_LOG(LogTemp, Display, TEXT("Registering Work Area..."))
	
	if (Area)
	{
		WorkAreas.AddUnique(Area);
		UE_LOG(LogTemp, Display, TEXT("Registered WorkArea"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WorkAreaManager RegisterWorkArea: Area not found."));
		return;
	}
	
	for (AInteractableBase* Interactable : Area->CustomerWorkstations)
	{
		if (!Interactable)
		{
			continue;
		}
		
		UInteractableWorkstationComponent* WSC = Interactable->FindComponentByClass<UInteractableWorkstationComponent>();

		RebuildWorkstationEntries(WSC);

		WSC->OnProductionSettingsChanged.AddUObject(this, &UWorkAreaManager::RebuildWorkstationEntries);
	}
}

void UWorkAreaManager::UnregisterWorkArea(AWorkAreaBase* Area)
{
	if (!Area) return;
	
	WorkAreas.Remove(Area);
	
	TSet<UInteractableWorkstationComponent*> WorkstationsToRemove;

	for (AInteractableBase* Interactable : Area->CustomerWorkstations)
	{
		if (!Interactable) continue;

		if (UInteractableWorkstationComponent* WSC =
			Interactable->FindComponentByClass<UInteractableWorkstationComponent>())
		{
			WorkstationsToRemove.Add(WSC);

			WSC->OnProductionSettingsChanged.RemoveAll(this);
		}
	}

	for (auto& Pair : TagToWorkstations)
	{
		TArray<FTaggedWorkstationEntry>& Entries = Pair.Value;

		Entries.RemoveAll([&](const FTaggedWorkstationEntry& Entry)
		{
			return WorkstationsToRemove.Contains(Entry.Workstation);
		});
	}
}

const TArray<AWorkAreaBase*>& UWorkAreaManager::GetAllWorkAreas() const
{
	return WorkAreas;
}

const AWorkAreaBase* UWorkAreaManager::GetWorkAreaByName(FName Name) const
{
	for (AWorkAreaBase* Area : WorkAreas)
	{
		if (Area && Area->WorkAreaName == Name)
		{
			return Area;
		}
	}
	return nullptr;
}

const TArray<FTaggedWorkstationEntry>* UWorkAreaManager::GetEntriesForTag(const FGameplayTag& Tag) const
{
	return TagToWorkstations.Find(Tag);
}

void UWorkAreaManager::RebuildWorkstationEntries(UInteractableWorkstationComponent* Workstation)
{
	UE_LOG(LogTemp, Display, TEXT("Rebuilding Workstation Entries"));
	
	if (!Workstation)
	{
		return;
	}

	for (const FWorkstationProductionSetting& Setting : Workstation->GetProductionSettings())
	{
		UPurchasableDefinition* Purchasable = Setting.Purchasable;
		if (!Purchasable) continue;

		const bool bValid = Setting.bEnabled && Setting.TargetAmount > 0;

		if (!bValid)
		{
			RemoveEntry(Workstation, Purchasable);
			continue;
		}

		if (EntryExists(Workstation, Purchasable))
		{
			//UpdateEntry(Workstation, Purchasable, Available);
		}
		else
		{
			AddEntry(Workstation, Purchasable);
		}
	}
	
	LogTagToWorkstationsMap(); // ToDo temp
}

void UWorkAreaManager::RemoveWorkstationEntries(UInteractableWorkstationComponent* Workstation, UPurchasableDefinition* Purchasable)
{
	if (!Workstation || !Purchasable) return;

	for (const FGameplayTag& Tag : Purchasable->ItemTags)
	{
		if (TArray<FTaggedWorkstationEntry>* Entries = TagToWorkstations.Find(Tag))
		{
			Entries->RemoveAll([&](const FTaggedWorkstationEntry& Entry)
			{
				return Entry.Workstation == Workstation &&
					   Entry.Purchasable == Purchasable;
			});
		}
	}
}

bool UWorkAreaManager::EntryExists(UInteractableWorkstationComponent* Workstation,
	UPurchasableDefinition* Purchasable) const
{
	if (!Workstation || !Purchasable)
	{
		return false;
	}

	for (const FGameplayTag& Tag : Purchasable->ItemTags)
	{
		if (!ShouldIncludeTag(Tag))
		{
			continue;
		}
		
		if (const TArray<FTaggedWorkstationEntry>* Entries = TagToWorkstations.Find(Tag))
		{
			for (const FTaggedWorkstationEntry& Entry : *Entries)
			{
				if (Entry.Workstation == Workstation && Entry.Purchasable == Purchasable)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void UWorkAreaManager::AddEntry(UInteractableWorkstationComponent* Workstation, UPurchasableDefinition* Purchasable)
{
	if (!Workstation || !Purchasable)
	{
		return;
	}

	for (const FGameplayTag& Tag : Purchasable->ItemTags)
	{
		if (!ShouldIncludeTag(Tag))
		{
			continue;
		}
		
		FTaggedWorkstationEntry Entry;
		Entry.Workstation = Workstation;
		Entry.Purchasable = Purchasable;

		TagToWorkstations.FindOrAdd(Tag).Add(Entry);
	}
}

void UWorkAreaManager::RemoveEntry(UInteractableWorkstationComponent* Workstation, UPurchasableDefinition* Purchasable)
{
	if (!Workstation || !Purchasable)
	{
		return;
	}

	for (const FGameplayTag& Tag : Purchasable->ItemTags)
	{
		if (!ShouldIncludeTag(Tag))
		{
			continue;
		}
		
		if (TArray<FTaggedWorkstationEntry>* Entries = TagToWorkstations.Find(Tag))
		{
			Entries->RemoveAll([&](const FTaggedWorkstationEntry& Entry)
			{
				return Entry.Workstation == Workstation &&
					   Entry.Purchasable == Purchasable;
			});
		}
	}
}

void UWorkAreaManager::UpdateEntry(UInteractableWorkstationComponent* Workstation, UPurchasableDefinition* Purchasable)
{
	if (!Workstation || !Purchasable)
	{
		return;
	}

	for (const FGameplayTag& Tag : Purchasable->ItemTags)
	{
		if (!ShouldIncludeTag(Tag))
		{
			continue;
		}
		
		if (TArray<FTaggedWorkstationEntry>* Entries = TagToWorkstations.Find(Tag))
		{
			for (FTaggedWorkstationEntry& Entry : *Entries)
			{
				if (Entry.Workstation == Workstation &&
					Entry.Purchasable == Purchasable)
				{
					
				}
			}
		}
	}
}

bool UWorkAreaManager::ShouldIncludeTag(const FGameplayTag& Tag) const
{
	if (Tag == MPGameplayTags::TAG_Purchasable ||
		Tag == MPGameplayTags::TAG_Purchasable_Food)
	{
		return false;
	}

	// Allow only sub-tags of Purchasable
	return Tag.MatchesTag(MPGameplayTags::TAG_Purchasable);
}

void UWorkAreaManager::LogTagToWorkstationsMap() const
{
	UE_LOG(LogTemp, Warning, TEXT("===== TagToWorkstations MAP LOG ====="));

	for (const TPair<FGameplayTag, TArray<FTaggedWorkstationEntry>>& Pair : TagToWorkstations)
	{
		const FGameplayTag& Tag = Pair.Key;
		const TArray<FTaggedWorkstationEntry>& Entries = Pair.Value;

		UE_LOG(LogTemp, Warning, TEXT("Tag: %s | Entries: %d"),
			*Tag.ToString(), Entries.Num());

		for (const FTaggedWorkstationEntry& Entry : Entries)
		{
			FString WorkstationName = TEXT("NULL");
			FString PurchasableName = TEXT("NULL");

			if (Entry.Workstation && Entry.Workstation->GetOwner())
			{
				WorkstationName = Entry.Workstation->GetOwner()->GetName();
			}

			if (Entry.Purchasable)
			{
				PurchasableName = Entry.Purchasable->GetName();
			}

			UE_LOG(LogTemp, Warning, TEXT("  -> Workstation: %s | Purchasable: %s"),
				*WorkstationName,
				*PurchasableName);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("===== END MAP LOG ====="));
}
