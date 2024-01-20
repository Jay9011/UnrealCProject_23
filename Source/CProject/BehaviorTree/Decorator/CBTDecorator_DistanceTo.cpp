#include "BehaviorTree/Decorator/CBTDecorator_DistanceTo.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/CAIController.h"
#include "Utilities/CheckMacros.h"

UCBTDecorator_DistanceTo::UCBTDecorator_DistanceTo()
{
	NodeName = "Distance To";
}

bool UCBTDecorator_DistanceTo::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	CheckNullResult(Controller, false);

	return CompareDistance(Controller->GetBlackboardComponent()->GetValueAsFloat(BlackboardKey.SelectedKeyName));
}

bool UCBTDecorator_DistanceTo::CompareDistance(float Distance) const
{
	switch (ArithmeticOperation) {
	case EArithmeticKeyOperation::Equal:
		{
			if (Distance == FloatValue)
				return true;
		}
		break;
	case EArithmeticKeyOperation::NotEqual:
		{
			if (Distance != FloatValue)
				return true;
		}
		break;
	case EArithmeticKeyOperation::GreaterOrEqual:
		{
			if (Distance >= FloatValue)
				return true;
		}
		break;
	case EArithmeticKeyOperation::LessOrEqual:
		{
			if (Distance <= FloatValue)
				return true;
		}
		break;
	case EArithmeticKeyOperation::Greater:
		{
			if (Distance > FloatValue)
				return true;
		}
		break;
	case EArithmeticKeyOperation::Less:
		{
			if (Distance < FloatValue)
				return true;
		}
		break;
	}

	return false;
}

bool UCBTDecorator_DistanceTo::CompareDistance(const FVector& InLocation, const FVector& InTargetLocation) const
{
	float Distance = FVector::Distance(InLocation, InTargetLocation);
	
	return CompareDistance(Distance);;
}
