#include "Weapons/Evades/CEvadeAction_Blade.h"

#include "Components/CEvadeComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/UDirectionalUtilities.h"

void UCEvadeAction_Blade::OnBeginEquip()
{
	Super::OnBeginEquip();
	EvadeComponent->OnBeginEvadeDelegate.Clear();
	EvadeComponent->OnBeginEvadeDelegate.AddDynamic(this, &UCEvadeAction_Blade::BeginEvade);
	EvadeComponent->OnEndEvadeDelegate.Clear();
	EvadeComponent->OnEndEvadeDelegate.AddDynamic(this, &UCEvadeAction_Blade::EndEvade);
}

void UCEvadeAction_Blade::OnUnequip()
{
	Super::OnUnequip();
	EvadeComponent->DelegateInit();
}

void UCEvadeAction_Blade::BeginEvade()
{
	Super::BeginEvade();

	EEvadeDir Dir = EEvadeDir::FRONT;

	FVector TargetDir = UDirectionalUtil::GetWorldDirectionFromInputAxis(OwnerCharacter, "MoveForward", "MoveRight");
	if(!TargetDir.IsNearlyZero())
	{
		FDirectionalValue DirectionalValue =  UDirectionalUtil::GetDirectionalValue(TargetDir, OwnerCharacter);

		if (DirectionalValue.Forward > 0.7f) Dir = EEvadeDir::FRONT;
		else if (DirectionalValue.Forward < -0.7f) Dir = EEvadeDir::BACK; 
		else if (DirectionalValue.Right > 0.7f) Dir = EEvadeDir::RIGHT;
		else if (DirectionalValue.Right < -0.7f) Dir = EEvadeDir::LEFT;
	}

	// 컨트롤러에 캐릭터의 방향을 고정시키지 않기 위해 기존의 ControlRotation을 저장하면서 고정을 푼다.
	MovementComponent->BackupControlRotation();
	MovementComponent->DisableControlRotation();
	
	// 입력 방향으로 캐릭터를 회전시킨다.
	if (!TargetDir.IsNearlyZero())
	{
		FVector TargetNormal = TargetDir.GetSafeNormal();

		FQuat TargetQuat;

		switch (Dir)
		{
		case EEvadeDir::FRONT:
			TargetQuat = FQuat::FindBetweenNormals(OwnerCharacter->GetActorForwardVector().GetSafeNormal(), TargetDir);
			break;
		case EEvadeDir::BACK:
			TargetQuat = FQuat::FindBetweenNormals(OwnerCharacter->GetActorForwardVector().GetSafeNormal(), -TargetDir);
			break;
		case EEvadeDir::LEFT:
			TargetQuat = FQuat::FindBetweenNormals(OwnerCharacter->GetActorRightVector().GetSafeNormal(), -TargetDir);
			break;
		case EEvadeDir::RIGHT:
			TargetQuat = FQuat::FindBetweenNormals(OwnerCharacter->GetActorRightVector().GetSafeNormal(), TargetDir);
			break;
		}

		OwnerCharacter->SetActorRotation(TargetQuat * OwnerCharacter->GetActorQuat());
	}
	
	EvadeDataPtr[static_cast<int32>(Dir)]->PlayMontage(OwnerCharacter);
}

void UCEvadeAction_Blade::EndEvade()
{
	Super::EndEvade();
	MovementComponent->RestoreControlRotation();
	StateComponent->SetIdleMode();
}
