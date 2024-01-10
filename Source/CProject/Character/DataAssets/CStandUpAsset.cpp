#include "Character/DataAssets/CStandUpAsset.h"

#include "Character/CAnimInstance.h"
#include "Components/CMovementComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"

void UCStandUpAsset::StandUp(EDir InDir, ACharacter* InOwner)
{
	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(InOwner->GetComponentByClass(UCMovementComponent::StaticClass()));
	CheckNull(MovementComponent);

	CheckFalse(MovementComponent->IsDown());

	UCAnimInstance* AnimInstance = Cast<UCAnimInstance>(InOwner->GetMesh()->GetAnimInstance());
	CheckNull(AnimInstance);

	if (StandUpMontage[(uint8)InDir] != nullptr)
	{
		AnimInstance->Montage_Play(StandUpMontage[(uint8)InDir]);
		return;
	}

	// 만약, 해당 방향의 기상 몽타주가 없다면, 제자리에서 기상한다.
	AnimInstance->Montage_Play(StandUpMontage[(uint8)EDir::InPlace]);
}
