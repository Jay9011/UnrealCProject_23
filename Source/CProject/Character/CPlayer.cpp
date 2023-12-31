#include "Character/CPlayer.h"
#include "Global.h"

#include "CAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CAirComponent.h"
#include "Components/CEvadeComponent.h"
#include "Components/CGuardComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/InputComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyDebugger/DebuggerComponent.h"
#include "Utilities/UDirectionalUtilities.h"
#include "Weapons/CWeaponStructures.h"

ACPlayer::ACPlayer()
{
#if DEBUG_DEFAULT_INFO
	PrimaryActorTick.bCanEverTick = true;
#endif
	
	SpringArm = this->CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetMesh());
	
	Camera = this->CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	MovementComponent = this->CreateDefaultSubobject<UCMovementComponent>("Movement");
	Weapon = this->CreateDefaultSubobject<UCWeaponComponent>("Weapon");
	
	Evade = this->CreateDefaultSubobject<UCEvadeComponent>("Evade");
	Air = this->CreateDefaultSubobject<UCAirComponent>("Air");

	//====================================================================================================
	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	TSubclassOf<UCAnimInstance> AnimInstance = ConstructorHelpers::FClassFinder<UCAnimInstance>(TEXT("AnimBlueprint'/Game/Character/ABP_BaseCharacter.ABP_BaseCharacter_C'")).Class;
	GetMesh()->SetAnimClass(AnimInstance);

#if WITH_EDITOR
	Debugger = CreateDefaultSubobject<UDebuggerComponent>("Debugger");
#endif
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->SetSpeed(ESpeedType::Run);
	MovementComponent->DisableControlRotation();

	HitMontage = Cast<UHitMontageComponent>(GetComponentByClass(UHitMontageComponent::StaticClass()));
	GuardComponent = Cast<UCGuardComponent>(GetComponentByClass(UCGuardComponent::StaticClass()));

#if DEBUG_DEFAULT_INFO
	Debugger->AddCollector(this);
#endif
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", MovementComponent, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", MovementComponent, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", MovementComponent, &UCMovementComponent::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", MovementComponent, &UCMovementComponent::OnVerticalLook);

	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, MovementComponent, &UCMovementComponent::OnWalk);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, MovementComponent, &UCMovementComponent::OnRun);
	
	PlayerInputComponent->BindAction("MainWeapon", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetMainWeaponMode);
	
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);
	
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SubAction_Pressed);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, Weapon, &UCWeaponComponent::SubAction_Released);
	
	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, Evade, &UCEvadeComponent::OnEvade);

	PlayerInputComponent->BindAction("CtrlAction", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::Ctrl_Pressed);
	PlayerInputComponent->BindAction("CtrlAction", EInputEvent::IE_Released, Weapon, &UCWeaponComponent::Ctrl_Released);
}

void ACPlayer::Falling()
{
	Super::Falling();

	CheckNull(Air);

	// 공중 상태 변화 전인 경우
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

void ACPlayer::PlayHittedMontage()
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
	
}// void ACPlayer::PlayHittedMontage()

void ACPlayer::AirSuspension(ACharacter& Character, ACharacter& Attacker)
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
float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Damaged.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damaged.Causer = DamageCauser;
	Damaged.Event = (FActionDamageEvent*)&DamageEvent;
	
	// 가드 상태인 경우
	if (GuardComponent != nullptr && !GuardComponent->IsUnBlocking())
	{
		// 가드가 가능한 상태인지 확인
		if (UDirectionalUtil::GetRotationFromTwoActors(this, Damaged.Character) >= 0.f)
		{
			GuardComponent->GuardSuccess(true, Damaged);
			return 0;
		}

		// TODO: 가드가 불가능하니 가드를 해제하고 피격 상태로 전환
	}
	
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damaged.Damage = Damage;

	State->SetHittedMode();
	
	return Damage;
}

void ACPlayer::ApplyDamage()
{
	Super::ApplyDamage();
}

void ACPlayer::LaunchEffect(const FRotator& InLookAtRotation)
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

void ACPlayer::LaunchAttacker(const FRotator& InLookAtRotation)
{
	Super::LaunchAttacker(InLookAtRotation);
}

void ACPlayer::ResetDamagedData(FDamagedData& DamagedData)
{
	Super::ResetDamagedData(DamagedData);
}

/*
 * 피격 상태 변화 후 처리
 */
void ACPlayer::Hitted()
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

void ACPlayer::Dead()
{
	Montages->Dead();
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Evade:
		Evade->OnBeginEvadeDelegate.Broadcast();
		break;
	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead: Dead(); break;
	}
}

void ACPlayer::End_Evade()
{
	Evade->OnEndEvadeDelegate.Broadcast();
}

void ACPlayer::End_Hitted()
{
	State->SetIdleMode();
}

void ACPlayer::End_Blocking()
{
	State->SetIdleMode();
}

void ACPlayer::End_Unprotected()
{
	State->SetIdleMode();
}

void ACPlayer::End_Dead()
{
}

#if DEBUG_DEFAULT_INFO
FDebugInfo ACPlayer::GetDebugInfo()
{
	FDebugInfo Info;
	Info.Priority = static_cast<int32>(DEBUG_NUMS::DEFAULT_INFO);

	// // InputDir의 X와 Y만 출력
	// FVector InputDir = {GetInputAxisValue("MoveForward"), GetInputAxisValue("MoveRight"), 0}; 
	// Info.Data.Add({FString::Printf(TEXT("Input Value: X=%.3f Y=%.3f"), InputDir.X, InputDir.Y), FColor::White});
	//
	// FVector Directional = UDirectionalUtil::GetWorldDirectionFromInputAxis(this, "MoveForward", "MoveRight");
	// Info.Data.Add({FString::Printf(TEXT("Character,Input Axis: X=%.3f Y=%.3f"), Directional.X, Directional.Y), FColor::White});
	//
	// InputDir.Normalize();
	// // 캐릭터의 각 방향 벡터를 구한다.
	// FVector Forward = GetActorForwardVector();
	// FVector Right = GetActorRightVector();
	// Forward.Z = 0;
	// Right.Z = 0;
	// Info.Data.Add({FString::Printf(TEXT("Forward: X=%.3f Y=%.3f , Right: X=%.3f Y=%.3f"), Forward.X, Forward.Y, Right.X, Right.Y), FColor::White});
	//
	// // 내적을 통해 더 가까운 방향을 구한다.
	// float ForwardDot = FVector::DotProduct(Forward, Directional);
	// float RightDot = FVector::DotProduct(Right, Directional);
	// Info.Data.Add({FString::Printf(TEXT("ForwardDot: %.3f RightDot: %.3f"), ForwardDot, RightDot), FColor::White});

	// Standing 타입 출력
	if(!!MovementComponent)
		Info.Data.Add({"Standing Type : " + StaticEnum<EStandingType>()->GetNameStringByIndex(static_cast<int32>(MovementComponent->GetStandingType())), FColor::White});
	
	// Air 상태 출력
	if (!!Air)
		Info.Data.Add({"Air Type : " + StaticEnum<EAirState>()->GetNameStringByIndex(static_cast<int32>(Air->GetAirState())), FColor::White});
	
	return Info;
}
#endif
