#include "BTTask_DecreaseBlackboardInt.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_DecreaseBlackboardInt::UBTTask_DecreaseBlackboardInt()
{
	NodeName = TEXT("Decrease Blackboard Int");
}

EBTNodeResult::Type UBTTask_DecreaseBlackboardInt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	if (FloatKey.SelectedKeyName == NAME_None)
	{
		return EBTNodeResult::Failed;
	}

	float CurrentValue = BlackboardComp->GetValueAsFloat(FloatKey.SelectedKeyName);

	CurrentValue -= DecreaseAmount;

	BlackboardComp->SetValueAsFloat(FloatKey.SelectedKeyName, CurrentValue);

	return EBTNodeResult::Succeeded;
}