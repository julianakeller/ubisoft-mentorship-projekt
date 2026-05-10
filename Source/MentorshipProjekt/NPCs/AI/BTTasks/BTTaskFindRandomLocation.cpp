// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskFindRandomLocation.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationSystemTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavFilters/NavigationQueryFilter.h"

UBTTaskFindRandomLocation::UBTTaskFindRandomLocation()
{
	NodeName = "Find Random Location";
}

EBTNodeResult::Type UBTTaskFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController){
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn->GetWorld());
	if (!NavSys)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation RandomLocation;
	
	FSharedConstNavQueryFilter QueryFilter;

	if (NavigationFilterClass)
	{
		QueryFilter = UNavigationQueryFilter::GetQueryFilter(
			*NavSys->GetDefaultNavDataInstance(),
			NavigationFilterClass
		);
	}

	if (NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), SearchRadius, RandomLocation, nullptr, QueryFilter))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKey.SelectedKeyName, RandomLocation.Location);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}