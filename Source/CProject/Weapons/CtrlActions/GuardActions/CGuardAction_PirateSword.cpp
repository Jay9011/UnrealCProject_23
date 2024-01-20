#include "Weapons/CtrlActions/GuardActions/CGuardAction_PirateSword.h"

#include "Components/CMovementComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CHelpers.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CWeaponStructures.h"

UCGuardAction_PirateSword::UCGuardAction_PirateSword()
{
}

void UCGuardAction_PirateSword::BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InWeapon, InAttachment);
}

void UCGuardAction_PirateSword::BlockingSuccess(FDamagedData& DamagedData)
{
	Super::BlockingSuccess(DamagedData);

	FVector HitLocation = DamagedData.Event->OverlappedComponent->GetComponentLocation();
	FVector HitDirection = HitLocation - Attachment->GetActorLocation();

	FTransform EffectTransform = FTransform(HitDirection.Rotation(), HitLocation, EffectScale);
	
	// 데미지를 입은 방향으로 이펙트를 생성한다.
	CHelpers::PlayEffect(OwnerCharacter->GetWorld(), MetalEffect, EffectTransform);

	// TODO:카메라 효과
}

void UCGuardAction_PirateSword::GuardBreak(FDamagedData& DamagedData)
{
	Super::GuardBreak(DamagedData);
}
