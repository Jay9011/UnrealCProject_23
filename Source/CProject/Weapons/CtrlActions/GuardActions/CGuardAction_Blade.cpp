#include "Weapons/CtrlActions/GuardActions/CGuardAction_Blade.h"

#include "CParryAction.h"
#include "Components/CGuardComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/TimelineComponent.h"
#include "Effect/PlayerEffectComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"
#include "Utilities/CHelpers.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CGuardMontageAsset.h"
#include "Weapons/CWeaponStructures.h"

UCGuardAction_Blade::UCGuardAction_Blade()
{
}

void UCGuardAction_Blade::BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InWeapon, InAttachment);
	if (ParryActionClass != nullptr)
	{
		ParryAction = NewObject<UCParryAction>(this, ParryActionClass);
		ParryAction->BeginPlay(OwnerCharacter, Weapon);
	}

}

void UCGuardAction_Blade::OnGuardSuccess(bool bSuccess, FDamagedData& DamagedData)
{
	Super::OnGuardSuccess(bSuccess, DamagedData);

	FVector HitLocation = DamagedData.Event->OverlappedComponent->GetComponentLocation();
	FVector HitDirection = HitLocation - Attachment->GetActorLocation();

	FTransform EffectTransform = FTransform(HitDirection.Rotation(), HitLocation, EffectScale);
	
	// 데미지를 입은 방향으로 이펙트를 생성한다.
	CHelpers::PlayEffect(OwnerCharacter->GetWorld(), MetalEffect, EffectTransform);

	// TODO:카메라 효과
}

void UCGuardAction_Blade::OnParryingSuccess(bool bSuccess, FDamagedData& DamagedData)
{
	Super::OnParryingSuccess(bSuccess, DamagedData);
	
	// 패링이 성공하면, 우선 예약된 패링 타이머들을 모두 종료한다.
	ClearParryingTimer();

	// 움직임을 제어하던 부분도 원래대로 돌려놓는다.
	MovementProcess();

	AttackerProcess(DamagedData.Character);
	UPlayerEffectComponent* PlayerEffect = Cast<UPlayerEffectComponent>(OwnerCharacter->GetComponentByClass(UPlayerEffectComponent::StaticClass()));
	if (PlayerEffect != nullptr)
	{
		TArray<AActor*> Actors;
		PlayerEffect->DilationEffect(OwnerCharacter, ParryingDilation, Actors, ParryingDilationCurve, PlayRate);
	}
	ParryingProcess(DamagedData.Character);
}

void UCGuardAction_Blade::AttackerProcess(ACharacter* InAttacker)
{
	CheckNull(InAttacker);

	UCWeaponComponent* AttackerWeaponComponent = Cast<UCWeaponComponent>(InAttacker->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNull(AttackerWeaponComponent);

	IIExcuteAction* CurrentAction = AttackerWeaponComponent->GetCurrentAction();
	if (CurrentAction != nullptr)
	{
		CurrentAction->End_Action();
	}

	UCStateComponent* AttackerStateComponent = Cast<UCStateComponent>(InAttacker->GetComponentByClass(UCStateComponent::StaticClass()));
	if (AttackerStateComponent != nullptr)
	{
		AttackerStateComponent->SetUnprotectedMode();
	}

	if (ParriedMontage != nullptr)
	{
		InAttacker->PlayAnimMontage(ParriedMontage, PlayRate);
	}
}

void UCGuardAction_Blade::ParryingProcess(ACharacter* InAttacker)
{
	EndBlockingMode();
	ParryAction->DoAction(InAttacker);
}

void UCGuardAction_Blade::ParryingDilationTimelineCallback(float Value)
{
	UGameplayStatics::SetGlobalTimeDilation(OwnerCharacter->GetWorld(), Value * (1.f - ParryingDilation) + ParryingDilation);
}
