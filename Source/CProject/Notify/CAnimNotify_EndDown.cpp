#include "Notify/CAnimNotify_EndDown.h"

#include "Utilities/CheckMacros.h"

FString UCAnimNotify_EndDown::GetNotifyName_Implementation() const
{
	return "EndDown";
}

void UCAnimNotify_EndDown::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(MeshComp->GetOwner()->GetComponentByClass(UCMovementComponent::StaticClass()));
	CheckNull(MovementComponent);

	switch (ToType)
	{
	case EStandingType::Crouch:
		MovementComponent->SetCrouchMode();
		break;
	case EStandingType::KnockDown:
		MovementComponent->SetKnockDownMode();
		break;
	case EStandingType::Standing:
	case EStandingType::Max:
	default:
		{
			MovementComponent->SetStandingMode();
		}
		break;
	}
}
