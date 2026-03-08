// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RelationshipRuleDataAsset.generated.h"

struct FRelationshipTransitionRule;
/**
 * 
 */
UCLASS()
class MENTORSHIPPROJEKT_API URelationshipRuleDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName RulesetName;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FRelationshipTransitionRule> TransitionRules;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("RelationshipRules", GetFName());
	}
};
