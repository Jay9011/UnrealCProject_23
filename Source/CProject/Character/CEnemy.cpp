#include "Character/CEnemy.h"

#include "Kismet/KismetMathLibrary.h"
#include "Utilities/CLog.h"
#include "Weapons/CWeaponStructures.h"
#include "Components/CAirComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "MyDebugger/DebuggerComponent.h"
#include "Utilities/CheckMacros.h"

ACEnemy::ACEnemy()
{
#if WITH_EDITOR
	Debugger = this->CreateDefaultSubobject<UDebuggerComponent>("Debugger");
#endif
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	Air = Cast<UCAirComponent>(GetComponentByClass(UCAirComponent::StaticClass()));
	HitMontage = Cast<UHitMontageComponent>(GetComponentByClass(UHitMontageComponent::StaticClass()));
	MovementComponent = Cast<UCMovementComponent>(GetComponentByClass(UCMovementComponent::StaticClass()));
	StatusComponent = Cast<UCStatusComponent>(GetComponentByClass(UCStatusComponent::StaticClass()));

#if WITH_EDITOR
	Debugger->AddCollector(this);
#endif
}

void ACEnemy::Falling()
{
	Super::Falling();

	CheckNull(Air);

	// 공중 상태 변경 전인 경우
	if (!Air->IsAir())
	{
		// 다운 상태에서 공중으로 띄워지는 경우
		if (!!MovementComponent && MovementComponent->IsKnockDown())
		{
			Air->SetAirborneMode();
		}
		else
		{
			Air->SetFallMode();
		}
	}
}

void ACEnemy::PlayHittedMontage()
{
	FHitData* HitData = Damaged.Event->HitData;
	
	if (MovementComponent != nullptr)
		HitData->bCanMove ? MovementComponent->Move() : MovementComponent->Stop();

	// 공중 상태에서 피격되는 경우
	if (!!Air && Air->IsAir())
	{
		if (HitMontage != nullptr)
			HitMontage->Play(EHitType::UpperNormal, 1.f, "HitPoint");

		AirSuspension(*this, *Damaged.Character);

		return;
	}
	
	if (!!HitMontage)
	{
		// 누워있는 상태
		if(!!MovementComponent && MovementComponent->IsKnockDown())
		{
			// 누워있는 상태에서 공중으로 띄워지는 경우
			if(HitData->DamagedLaunch.Z >= MinimumLiftZ)
			{
				if(Air != nullptr)
					Air->SetAirborneMode();
				MovementComponent->SetStandingMode();
				HitMontage->Play(EHitType::UpperNormal);
			}
			else
			{
				HitMontage->Play(EHitType::FrontDownLight);				
			}
			
			return;
		}// if(MovementComponent->IsKnockDown())

		
		HitMontage->Play(HitData->HitType, HitData->PlayRate);
		
	}// if (!!HitMontage)
	
}// void ACEnemy::PlayHittedMontage()

void ACEnemy::AirSuspension(ACharacter& Character, ACharacter& Attacker)
{
	FHitData* HitData = Damaged.Event->HitData;
	
	if (HitData->bSuspensionInAir && !!Air)
	{
		Air->SetGravityScale(HitData->GravityData.GravityScale, HitData->GravityData.SelfVelocity, HitData->GravityData.RecoveryTime);
	}

	if (HitData->bAttackerSuspensionAir)
	{
		UCAirComponent* AttackerAir = Cast<UCAirComponent>(Attacker.GetComponentByClass(UCAirComponent::StaticClass()));
		if (!!AttackerAir)
		{
			AttackerAir->SetGravityScale(HitData->AttackerGravityData.GravityScale, HitData->AttackerGravityData.SelfVelocity, HitData->AttackerGravityData.RecoveryTime);
		}
	}
}

/*
 * 데미지 처리 전 데미지 계산이나 여러 전처리를 함
 */
float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damaged.Damage = Damage;
	Damaged.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damaged.Causer = DamageCauser;
	Damaged.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetHittedMode();
	
	return Damage;
}

/*
 * 피격 상태 변화 후 처리
 */
void ACEnemy::Hitted()
{
	// 데미지 처리
	if (!!StatusComponent)
	{
		StatusComponent->Damage(Damaged.Damage);
	}
	Damaged.Damage = 0;

	// 피격 효과 처리
	if (!!Damaged.Event && !!Damaged.Event->HitData)
	{
		FHitData* HitData = Damaged.Event->HitData;

		PlayHittedMontage();
		HitData->PlayHitStop(GetWorld());
		HitData->PlaySoundWave(this);
		HitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Damaged.Character->GetActorLocation());
		LookAtRotation.Pitch = GetActorRotation().Pitch;
		LookAtRotation.Roll = GetActorRotation().Roll;
		
		// 밀려나는 효과 (사망일때에는 처리 안함)
		if (!StatusComponent->IsDead())
		{
			FVector LaunchPower = HitData->DamagedLaunch;
			
			// 만약 SuspensionInAir가 true면 XY축에 대해서만 밀려나게 한다.
			if (HitData->bSuspensionInAir)
			{
				LaunchPower = FVector::ZeroVector;
			}
			
			FVector RotatedLaunch = LookAtRotation.RotateVector(LaunchPower);
			LaunchCharacter({-RotatedLaunch.X, -RotatedLaunch.Y, RotatedLaunch.Z}, false, false);

			// 만약 isAirborne이 true면 피격으로 인해 캐릭터가 뒤로 넘어지는 효과를 준다.
			if (!!Air && HitData->isAirborne)
			{
				Air->SetAirborneMode();
			}

			// 공격자 방향으로 회전
			SetActorRotation(LookAtRotation);

			// 히트시 공격자에게 가해지는 효과
			if (HitData->AttackerLaunch != FVector::ZeroVector)
			{
				FVector RotatedAttackerLaunch = LookAtRotation.RotateVector(HitData->AttackerLaunch);
				Damaged.Character->LaunchCharacter({RotatedAttackerLaunch.X, RotatedAttackerLaunch.Y, RotatedAttackerLaunch.Z}, true, true);
			}
		}
	}

	// 중복 피격 방지
	Damaged.Character = nullptr;
	Damaged.Causer = nullptr;
	Damaged.Event = nullptr;
	
	if (StatusComponent->IsDead())
	{
		State->SetDeadMode();
	}
}

void ACEnemy::End_Hitted()
{
	State->SetIdleMode();
}

/*
 * 사망 상태 변화 후 처리
 */
void ACEnemy::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Montages->Dead();
}

void ACEnemy::End_Dead()
{
	Destroy();
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead: Dead(); break;
	}
}
#if WITH_EDITOR
  
bool ACEnemy::IsDebugEnable()
{
	return true;
}

FDebugInfo ACEnemy::GetDebugInfo()
{
	FDebugInfo Info;
	Info.Priority = -1;

	// Standing 타입 출력
	if(!!MovementComponent)
		Info.Data.Add({"Standing Type : " + StaticEnum<EStandingType>()->GetNameStringByIndex(static_cast<int32>(MovementComponent->GetStandingType())), FColor::White});
	
	// Air 상태 출력
	if (!!Air)
		Info.Data.Add({"Air Type : " + StaticEnum<EAirState>()->GetNameStringByIndex(static_cast<int32>(Air->GetAirState())), FColor::White});
	
	return Info;
}

#endif
