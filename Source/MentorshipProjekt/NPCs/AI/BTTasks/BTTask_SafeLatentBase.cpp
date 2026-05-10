// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SafeLatentBase.h"

UBTTask_SafeLatentBase::UBTTask_SafeLatentBase()
{
	bCreateNodeInstance = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_SafeLatentBase::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FinishTaskSafe(EBTNodeResult::Aborted);

	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_SafeLatentBase::StartTimeout(UBehaviorTreeComponent& OwnerComp)
{
	CachedOwnerComp = &OwnerComp;

	if (!bUseTimeout)
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		TimeoutHandle,
		this,
		&UBTTask_SafeLatentBase::OnTaskTimeout,
		MaxTaskDuration,
		false);
	
	UE_LOG(LogTemp, Warning, TEXT("BTTask %s timeout started: %.2fs"), *GetName(), MaxTaskDuration);
}

void UBTTask_SafeLatentBase::OnTaskTimeout()
{
	UE_LOG(LogTemp, Warning, TEXT("%s timed out"), *GetName());

	FinishTaskSafe(EBTNodeResult::Failed);
}

void UBTTask_SafeLatentBase::FinishTaskSafe(EBTNodeResult::Type Result)
{
	if (!CachedOwnerComp || !CachedOwnerComp->IsRunning())
	{
		return;
	}
	
	if (bTaskFinished)
	{
		return;
	}

	bTaskFinished = true;

	GetWorld()->GetTimerManager().ClearTimer(TimeoutHandle);

	CleanupTask();

	if (!CachedOwnerComp)
	{
		return;
	}

	UBehaviorTreeComponent* BTC = CachedOwnerComp;
	CachedOwnerComp = nullptr;

	FinishLatentTask(*BTC, Result);
}

void UBTTask_SafeLatentBase::CleanupTask()
{
}
