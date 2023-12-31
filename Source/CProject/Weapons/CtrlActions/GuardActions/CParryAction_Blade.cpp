#include "Weapons/CtrlActions/GuardActions/CParryAction_Blade.h"

#include "Character/CBaseCharacter.h"
#include "Components/CCameraRotationComponent.h"
#include "Components/CMovementComponent.h"
#include "Utilities/CheckMacros.h"

void UCParryAction_Blade::BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon)
{
	Super::BeginPlay(InOwner, InWeapon);

	
	if (OwnerCharacter != nullptr)
	{
		CameraRotationComponent = Cast<UCCameraRotationComponent>(OwnerCharacter->GetComponentByClass(UCCameraRotationComponent::StaticClass()));
	}
}

void UCParryAction_Blade::End_DoAction()
{
	Super::End_DoAction();
}

void UCParryAction_Blade::End_Action()
{
	Super::End_Action();
	
	MovementComponent->RestoreControlRotation();

	if (CameraRotationComponent->IsPlaying())
		CameraRotationComponent->Stop();
	
	End_DoAction();
}

void UCParryAction_Blade::DoAction(ACharacter* InAttacker)
{
	Super::DoAction(InAttacker);

	CheckNull(Attacker);
	
	Super::DoAction_Implementation();

	MovementComponent->BackupControlRotation();
	MovementComponent->DisableControlRotation();

	FRotator DestRotator = OwnerBaseCharacter->LookAt(Attacker);

	if (DoActionDatas.Num() > 0)
		DoActionDatas[0].DoAction(OwnerCharacter);

	CameraRotationComponent->Play(DestRotator, CameraRotationMultiplier, PlayRate, CameraRotationCurve);
}

void UCParryAction_Blade::OnAttachmentEndCollision()
{
	DamagedCharacters.Empty();
}

void UCParryAction_Blade::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther,
                                                   UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                   const FHitResult& SweepResult)
{
	CheckFalse(Weapon->GetCurrentAction() == this);
	CheckNull(InOther);

	// 중복 피격 방지
	{
		if (DamagedCharacters.Contains(InOther))
			return;
		
		DamagedCharacters.Add(InOther);
	}

	// 데미지 처리
	DoActionDatas[0].DamagedData.SendDamage(InAttacker, InAttackCauser, InOther, OverlappedComponent);
}
