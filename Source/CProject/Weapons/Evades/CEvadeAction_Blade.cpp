#include "Weapons/Evades/CEvadeAction_Blade.h"

#include "CGameMode.h"
#include "Components/CEvadeComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CJustEvadeComponent.h"
#include "GameFramework/Character.h"
#include "Parts/CGhostTrailComponent.h"
#include "Utilities/CTraceUtil.h"
#include "Utilities/UDirectionalUtilities.h"

void UCEvadeAction_Blade::BeginPlay(ACharacter* InOwner, TArray<FEvadeData>& InData)
{
	Super::BeginPlay(InOwner, InData);

	
	// 각종 설정이 포함된 게임 모드를 가져온다.
	GameMode = Cast<ACGameMode>(OwnerCharacter->GetWorld()->GetAuthGameMode());
	
	// JustEvade 컴포넌트를 가져온다.
	JustEvadeComponent = OwnerCharacter->FindComponentByClass<UCJustEvadeComponent>();

}

void UCEvadeAction_Blade::OnBeginEquip()
{
	Super::OnBeginEquip();
	
	if (EvadeComponent == nullptr)
		return;
	
	EvadeComponent->OnBeginEvadeDelegate.Clear();
	EvadeComponent->OnBeginEvadeDelegate.AddDynamic(this, &UCEvadeAction_Blade::BeginEvade);
	EvadeComponent->OnEndEvadeDelegate.Clear();
	EvadeComponent->OnEndEvadeDelegate.AddDynamic(this, &UCEvadeAction_Blade::EndEvade);
}

void UCEvadeAction_Blade::OnUnequip()
{
	Super::OnUnequip();

	if (EvadeComponent == nullptr)
		return;
	
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

	// 회피 애니메이션을 재생한다.
	MovementComponent->OffCrouch();
	EvadeDataPtr[static_cast<int32>(Dir)]->PlayMontage(OwnerCharacter);

	// Just 회피를 체크한다.
	if (JustEvadeComponent != nullptr && CheckJustEvade())
	{
		JustEvadeStart();
	}
}

void UCEvadeAction_Blade::EndEvade()
{
	Super::EndEvade();
	MovementComponent->RestoreControlRotation();
}

void UCEvadeAction_Blade::JustEvadeStart()
{
	JustEvadeComponent->StartJustEvade(Dilation, Duration, true, TrailInterval, TrailFirstDelay, TrailDuration);
	FTimerDelegate JustEvadeTimerDelegate;
	JustEvadeTimerDelegate.BindLambda([this]()
	{
		JustEvadeEnd();
	});
	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(JustEvadeTimerHandle, JustEvadeTimerDelegate, Duration, false);
}

void UCEvadeAction_Blade::JustEvadeEnd()
{
	if (JustEvadeComponent != nullptr && JustEvadeComponent->IsJustTime())
	{
		JustEvadeComponent->EndJustEvade();
	}
}

bool UCEvadeAction_Blade::CheckJustEvade()
{
	if (GameMode == nullptr)
		return false;

	TEnumAsByte<ETraceTypeQuery> TraceTypeQuery = GameMode->GetEvadableType();
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerCharacter);

	FHitResult HitResult;

	return UCTraceUtil::SingleTraceSphereOtherGroupTraceChannel(OwnerCharacter, CheckRadius, TraceTypeQuery, IgnoreActors, HitResult);
}
