#include "Weapons/DoSubActions/CDoSubAction_Blade_AirToAirDash.h"

#include "Components/CAirComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CPathComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CAttachment.h"

void UCDoSubAction_Blade_AirToAirDash::BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACharacter* InOwner,
                                                 ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwnerWeaponAsset, InOwner, InAttachment, InDoAction);

	CheckNull(InOwner);
	OwnerPathComponent = Cast<UCPathComponent>(InOwner->GetComponentByClass(UCPathComponent::StaticClass()));

	// 충돌체 관련 함수 바인딩
	if (Attachment != nullptr)
	{
		Attachment->OnAttachmentBeginCollision.AddDynamic(this, &UCDoSubAction_Blade_AirToAirDash::OnAttachmentBeginCollision);
		Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCDoSubAction_Blade_AirToAirDash::OnAttachmentEndCollision);
		Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCDoSubAction_Blade_AirToAirDash::OnAttachmentBeginOverlap);
		Attachment->OnAttachmentEndOverlap.AddDynamic(this, &UCDoSubAction_Blade_AirToAirDash::OnAttachmentEndOverlap);
	}

	ActionData.InitActionData();
}

void UCDoSubAction_Blade_AirToAirDash::Pressed()
{
	Super::Pressed();

	CheckNull(TargetHitResult.Actor.Get());
	CheckNull(OwnerPathComponent);

	UCAirComponent* TargetAirComponent = Cast<UCAirComponent>(TargetHitResult.Actor.Get()->GetComponentByClass(UCAirComponent::StaticClass()));

	if (TargetAirComponent != nullptr)
	{
		TargetAirComponent->SetGravityScale(GravityScale, Velocity, RecoveryTime);
	}

	OwnerPathComponent->MakePath(OwnerCharacter->GetActorLocation(), TargetHitResult.Actor->GetActorLocation(), ESplineCoordinateSpace::World, false);
	OwnerPathComponent->MoveAlongPath(Speed, Rate, PathCurve);

	
	StateComponent->SetActionMode();
	StateComponent->OnSubActionMode();

	OwnerWeaponAsset->SetCurrentAction(this);
	if (ActionData.ActionData.Num() > 0)
		ActionData.ActionData[0].DoAction(OwnerCharacter);
}

void UCDoSubAction_Blade_AirToAirDash::End_Action()
{
	Super::End_Action();

	StateComponent->SetIdleMode();
	StateComponent->OffSubActionMode();

	MovementComponent->Move();
	MovementComponent->DisableFixedCamera();

	OwnerWeaponAsset->UpdateActions();
}

void UCDoSubAction_Blade_AirToAirDash::OnAttachmentBeginCollision()
{
}

void UCDoSubAction_Blade_AirToAirDash::OnAttachmentEndCollision()
{
	ActionData.DamagedCharacters.Empty();
}

void UCDoSubAction_Blade_AirToAirDash::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckFalse(OwnerWeaponAsset->GetCurrentAction() == this);
	CheckNull(InOther);

	// 중복 피격 방지
	{
		if (ActionData.DamagedCharacters.Contains(InOther))
			return;
		
		ActionData.DamagedCharacters.Add(InOther);
	}

	// 데미지 처리
	ActionData.ActionData[0].DamagedData.SendDamage(InAttacker, InAttackCauser, InOther);
}

void UCDoSubAction_Blade_AirToAirDash::OnAttachmentEndOverlap(ACharacter* InAttacker, ACharacter* InOther)
{
}
