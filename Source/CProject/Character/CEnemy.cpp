#include "Character/CEnemy.h"

#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utilities/CLog.h"
#include "Weapons/CWeaponStructures.h"
#include "Components/CAirComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CGuardComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CMovementComponent.h"
#include "MyDebugger/DebuggerComponent.h"
#include "Utilities/CheckMacros.h"
#include "Utilities/UDirectionalUtilities.h"

ACEnemy::ACEnemy()
{
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	Air = Cast<UCAirComponent>(GetComponentByClass(UCAirComponent::StaticClass()));
	HitMontage = Cast<UHitMontageComponent>(GetComponentByClass(UHitMontageComponent::StaticClass()));
	MovementComponent = Cast<UCMovementComponent>(GetComponentByClass(UCMovementComponent::StaticClass()));
	
#if WITH_EDITOR
	Debugger = Cast<UDebuggerComponent>(GetComponentByClass(UDebuggerComponent::StaticClass()));
#endif
#if DEBUG_DEFAULT_INFO
	if (Debugger != nullptr)
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
		{
			HitMontage->Play(EHitType::UpperNormal, 1.f, "HitPoint");
			UE_LOG(LogTemp, Log, TEXT("PlayHittedMontage : UpperNormal"));
		}

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
	Damaged.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damaged.Causer = DamageCauser;
	Damaged.Event = (FActionDamageEvent*)&DamageEvent;

	UCGuardComponent* GuardComponent = Cast<UCGuardComponent>(GetComponentByClass(UCGuardComponent::StaticClass()));
	
	// 가드 상태인 경우
	if (GuardComponent != nullptr && !GuardComponent->IsUnGuard())
	{
		// 가드가 가능한 상태인지 확인
		if (UDirectionalUtil::GetRotationFromTwoActors(this, Damaged.Character) >= 0.f)
		{
			GuardComponent->EvaluateGuard(true, Damaged);
			return 0;
		}
	}
	
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damaged.Damage = Damage;

	State->SetHittedMode();
	
	return Damage;
}

void ACEnemy::ApplyDamage()
{
	Super::ApplyDamage();
}

void ACEnemy::LaunchEffect(const FRotator& InLookAtRotation)
{
	FHitData* HitData = Damaged.Event->HitData;
	
	FVector LaunchPower = HitData->DamagedLaunch;
			
	// 만약 SuspensionInAir가 true면 XY축에 대해서만 밀려나게 한다.
	if (HitData->bSuspensionInAir)
	{
		LaunchPower = FVector::ZeroVector;
	}
			
	FVector RotatedLaunch = InLookAtRotation.RotateVector(LaunchPower);
	LaunchCharacter({-RotatedLaunch.X, -RotatedLaunch.Y, RotatedLaunch.Z}, false, false);

	// 만약 isAirborne이 true면 피격으로 인해 캐릭터가 뒤로 넘어지는 효과를 준다.
	if (!!Air && HitData->isAirborne)
	{
		Air->SetAirborneMode();
	}
}

void ACEnemy::LaunchAttacker(const FRotator& InLookAtRotation)
{
	Super::LaunchAttacker(InLookAtRotation);
}

void ACEnemy::ResetDamagedData(FDamagedData& DamagedData)
{
	Super::ResetDamagedData(DamagedData);
}

/*
 * 피격 상태 변화 후 처리
 */
void ACEnemy::Hitted()
{
	ApplyDamage();

	// 피격 효과 처리
	if (!!Damaged.Event && !!Damaged.Event->HitData)
	{
		FHitData* HitData = Damaged.Event->HitData;

		PlayHittedMontage();
		
		HitData->PlayHitStop(GetWorld());
		HitData->PlaySoundWave(this);
		HitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

		// 밀려나는 효과 (사망일때에는 처리 안함)
		if (!Status->IsDead())
		{
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Damaged.Character->GetActorLocation());
			LookAtRotation.Pitch = GetActorRotation().Pitch;
			LookAtRotation.Roll = GetActorRotation().Roll;

			LaunchEffect(LookAtRotation);
			// 공격자 방향으로 회전
			SetActorRotation(LookAtRotation);
			LaunchAttacker(LookAtRotation);
		}
	}

	// 중복 피격 방지
	ResetDamagedData(Damaged);
	
	if (Status->IsDead())
	{
		State->SetDeadMode();
	}
}

/*
 * 사망 상태 변화 후 처리
 */
void ACEnemy::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Montages->Dead();
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead: Dead(); break;
	}
}

void ACEnemy::End_Hitted()
{
	State->SetIdleMode();
	MovementComponent->Move();
}

void ACEnemy::End_Blocking()
{
	State->SetIdleMode();
}

void ACEnemy::End_Unprotected()
{
	State->SetIdleMode();
}

void ACEnemy::End_Dead()
{
	Destroy();
}

#if DEBUG_DEFAULT_INFO
bool ACEnemy::IsDebugEnable()
{
	return true;
}

FDebugInfo ACEnemy::GetDebugInfo()
{
	FDebugInfo Info;
	Info.Priority = static_cast<int32>(DEBUG_NUMS::DEFAULT_INFO);

	// Standing 타입 출력
	if(!!MovementComponent)
		Info.Data.Add({"Standing Type : " + StaticEnum<EStandingType>()->GetNameStringByIndex(static_cast<int32>(MovementComponent->GetStandingType())), FColor::White});
	
	// Air 상태 출력
	if (!!Air)
		Info.Data.Add({"Air Type : " + StaticEnum<EAirState>()->GetNameStringByIndex(static_cast<int32>(Air->GetAirState())), FColor::White});
	
	return Info;
}

#endif
