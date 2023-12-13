#include "Weapons/DoSubActions/Blade/CDoSubAction_Blade_GndToDown.h"

#include "Components/CAirComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CPathComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"
#include "Utilities/UDirectionalUtilities.h"
#include "Weapons/CAttachment.h"

void UCDoSubAction_Blade_GndToDown::BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACharacter* InOwner,
                                              ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwnerWeaponAsset, InOwner, InAttachment, InDoAction);

	CheckNull(InOwner);
	OwnerPathComponent = Cast<UCPathComponent>(InOwner->GetComponentByClass(UCPathComponent::StaticClass()));

	if (Attachment != nullptr)
	{
		Attachment->OnAttachmentBeginCollision.AddDynamic(this, &UCDoSubAction_Blade_GndToDown::OnAttachmentBeginCollision);
		Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCDoSubAction_Blade_GndToDown::OnAttachmentEndCollision);
		Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCDoSubAction_Blade_GndToDown::OnAttachmentBeginOverlap);
		Attachment->OnAttachmentEndOverlap.AddDynamic(this, &UCDoSubAction_Blade_GndToDown::OnAttachmentEndOverlap);
	}
	
	ActionData.InitActionData();
}

void UCDoSubAction_Blade_GndToDown::Pressed()
{
	Super::Pressed();

	CheckNull(TargetHitResult.Actor.Get());
	CheckNull(OwnerPathComponent);

	FVector EndPoint;
	UCPositionUtil::GetEndPointExcludingRadius(OwnerCharacter, TargetHitResult.Actor.Get(), EndPoint);

	// 중간에 이동할 높이에 대한 벡터를 구한다. (점프)
	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation = TargetHitResult.Actor->GetActorLocation();
	FVector MidPoint = (TargetLocation + OwnerLocation) * 0.5f;
	MidPoint.Z = MidHeight;

	OwnerPathComponent->MakePath(OwnerLocation, EndPoint, ESplineCoordinateSpace::World, true, MidPoint);
	OwnerCharacter->Jump();
	OwnerPathComponent->MoveAlongPath(Speed, Rate, PathCurve);

	
	StateComponent->SetActionMode();
	StateComponent->OnSubActionMode();
	
	OwnerWeaponAsset->SetCurrentAction(this);
	if (ActionData.ActionData.Num() > 0)
		ActionData.ActionData[0].DoAction(OwnerCharacter);
}

void UCDoSubAction_Blade_GndToDown::End_Action()
{
	Super::End_Action();

	StateComponent->SetIdleMode();
	StateComponent->OffSubActionMode();

	MovementComponent->Move();
	MovementComponent->DisableFixedCamera();

	OwnerWeaponAsset->UpdateActions();
}

void UCDoSubAction_Blade_GndToDown::OnAttachmentBeginCollision()
{
	
}

void UCDoSubAction_Blade_GndToDown::OnAttachmentEndCollision()
{
	ActionData.DamagedCharacters.Empty();
}

void UCDoSubAction_Blade_GndToDown::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
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

void UCDoSubAction_Blade_GndToDown::OnAttachmentEndOverlap(ACharacter* InAttacker, ACharacter* InOther)
{
}
