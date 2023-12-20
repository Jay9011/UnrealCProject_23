#include "Weapons/DoSubActions/CDoSubAction_Blade.h"

#include "Character/CBaseCharacter.h"
#include "Components/CJustEvadeComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Interface/FHitResultInterface.h"
#include "Utilities/CTraceUtil.h"
#include "Utilities/UDirectionalUtilities.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponObject.h"

void UCDoSubAction_Blade::BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment,
                                    UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InWeapon, InAttachment, InDoAction);

	AirComponent = Cast<UCAirComponent>(OwnerCharacter->GetComponentByClass(UCAirComponent::StaticClass()));

	if(NormalGroundActionClass != nullptr)
	{
		NormalGroundAction = NewObject<UCDoSubAction>(this, NormalGroundActionClass);
		NormalGroundAction->BeginPlay(InOwner, Weapon, InAttachment, InDoAction);
	}

	if (AirToAirActionClass != nullptr)
	{
		AirToAirAction = NewObject<UCDoSubAction>(this, AirToAirActionClass);
		AirToAirAction->BeginPlay(InOwner, Weapon, InAttachment, InDoAction);
	}

	if (AirToGroundActionClass != nullptr)
	{
		AirToGroundAction = NewObject<UCDoSubAction>(this, AirToGroundActionClass);
		AirToGroundAction->BeginPlay(InOwner, Weapon, InAttachment, InDoAction);
	}

	if (GroundToDownActionClass != nullptr)
	{
		GroundToDownAction = NewObject<UCDoSubAction>(this, GroundToDownActionClass);
		GroundToDownAction->BeginPlay(InOwner, Weapon, InAttachment, InDoAction);
	}

	if (EvadeActionClass != nullptr)
	{
		EvadeAction = NewObject<UCDoSubAction>(this, EvadeActionClass);
		EvadeAction->BeginPlay(InOwner, Weapon, InAttachment, InDoAction);
	}
}

void UCDoSubAction_Blade::Pressed()
{
	Super::Pressed();

	if (MovementComponent->IsCrouch())
		MovementComponent->OffCrouch();

	// 서 있는 상태에서 SubAction을 실행한 경우
	if (MovementComponent->IsStand())
	{
		if (StateComponent->IsIdleMode())
		{
			// 공중에 있는 상태에서 공격을 실행한 경우
			if (AirComponent->IsAerialStance())
			{
				PerformAerialStanceAction();
				return;
			}

			// 지상에 있는 상태에서 공격을 실행한 경우
			if (!AirComponent->IsAir())
			{
				PerformGroundStanceAction();
				return;
			}
		}
		// Idle 상태가 아니라면
		else
		{
			// JustMode 액션
			if (TryJustEvade())
			{
				return;
			}
			
			switch (StateComponent->GetType())
			{
			case EStateType::Evade :
				break;
			case EStateType::Action :
				if (NormalGroundAction != nullptr)
					NormalGroundAction->Pressed();
				break;
			}
		}
	}

	
} // void UCDoSubAction_Blade::Pressed()

void UCDoSubAction_Blade::Released()
{
	Super::Released();

	UCDoSubAction* CurrentAction = Cast<UCDoSubAction>(Weapon->GetCurrentAction());
	if (CurrentAction == nullptr)
		return;

	CurrentAction->Released();
	
} // void UCDoSubAction_Blade::Released()

void UCDoSubAction_Blade::PerformAerialStanceAction()
{
	TArray<AActor*> IgnoreActors;
	IgnoreActors.AddUnique(OwnerCharacter);
	FHitResult HitResult;
	if(!UCTraceUtil::TraceForwardNearEnemyByProfile(OwnerCharacter, AirTraceLength, AirTraceRadius, AirTraceProfile, IgnoreActors, HitResult))
		return;

	const ACharacter* TargetCharacter = Cast<ACharacter>(HitResult.GetActor());
	const UCharacterMovementComponent* TargetMovementComponent = Cast<UCharacterMovementComponent>(TargetCharacter->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	// 만약 타겟이 공중에 있다면
	if (TargetMovementComponent->IsFalling())
	{
		if(AirToAirAction != nullptr)
		{
			// IFHitResult 인터페이스가 구현되어 있으면 FHitResult를 전달한다.
			IFHitResultInterface* IFHitResult = Cast<IFHitResultInterface>(AirToAirAction);
			if (IFHitResult != nullptr)
			{
				IFHitResult->SetFHitResult(HitResult);
			}
			AirToAirAction->Pressed();
		}
		return;
	}
	// 만약 타겟이 지상에 있다면
	if(AirToGroundAction != nullptr)
	{
		// IFHitResult 인터페이스가 구현되어 있으면 FHitResult를 전달한다.
		IFHitResultInterface* IFHitResult = Cast<IFHitResultInterface>(AirToGroundAction);
		if (IFHitResult != nullptr)
		{
			IFHitResult->SetFHitResult(HitResult);
		}
		AirToGroundAction->Pressed();
	}

	return;
}

void UCDoSubAction_Blade::PerformGroundStanceAction()
{
	TArray<AActor*> IgnoreActors;
	IgnoreActors.AddUnique(OwnerCharacter);
	FHitResult HitResult;

	// 만약 지상에 다운 상태인 적이 있다면
	if (TraceForwardNearDownEnemyByProfile(GroundTraceLength, GroundTraceRadius, GroundTraceProfile, IgnoreActors, HitResult))
	{
		if(GroundToDownAction != nullptr)
		{
			// IFHitResult 인터페이스가 구현되어 있으면 FHitResult를 전달한다.
			IFHitResultInterface* IFHitResult = Cast<IFHitResultInterface>(GroundToDownAction);
			if (IFHitResult != nullptr)
			{
				IFHitResult->SetFHitResult(HitResult);
			}
			GroundToDownAction->Pressed();
		}

		return;
	}

	// 만약 지상에 다운 상태인 적이 없고, 공중 상태의 적도 없다면
	if (NormalGroundAction != nullptr)
		NormalGroundAction->Pressed();

	return;
}

bool UCDoSubAction_Blade::TryJustEvade()
{
	UCJustEvadeComponent* JustEvadeComponent = Cast<UCJustEvadeComponent>(OwnerCharacter->GetComponentByClass(UCJustEvadeComponent::StaticClass()));

	if(JustEvadeComponent != nullptr
		&& JustEvadeComponent->IsJustTime()
		&& EvadeAction != nullptr)
	{
		JustEvadeComponent->EndJustEvade();
		EvadeAction->Pressed();
	}
	
	return false;
}

bool UCDoSubAction_Blade::TraceForwardNearDownEnemyByProfile(const float InLength, const float InRadius, const FName& InProfile, const TArray<AActor*>& InIgnoreActors, FHitResult& OutHitResult) const
{
	FVector Start, End;
	UCPositionUtil::GetScreenCenterLocationWithACharacter(OwnerCharacter, InLength, InRadius, Start, End);

	TArray<FHitResult> HitResult;
	if(!UCTraceUtil::TraceForwardOtherGroupByProfile(OwnerCharacter, Start, End, InRadius, InProfile, InIgnoreActors, HitResult))
		return false;

	TArray<FHitResult> DownHitResult;
	for (const FHitResult& Hit : HitResult)
	{
		// 타겟이 다운상태인지 확인
		const ACBaseCharacter* HitCharacter = Cast<ACBaseCharacter>(Hit.GetActor());
		if (HitCharacter == nullptr)
			continue;

		const UCMovementComponent* HitMovementComponent = Cast<UCMovementComponent>(HitCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));
		if (HitMovementComponent == nullptr || !HitMovementComponent->IsDown())
			continue;

		DownHitResult.Add(Hit);
	}

	if (DownHitResult.Num() <= 0)
		return false;

	// 다운 상태의 적들 중 가장 가까운 적을 찾는다.
	float MinDistance = TNumericLimits<float>::Max();
	for (const FHitResult& Hit : DownHitResult)
	{
		FVector HitLocation = Hit.Location;
		FVector OwnerLocation = OwnerCharacter->GetActorLocation();
		const float Distance = FVector::DistSquared(HitLocation, OwnerLocation);

		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			OutHitResult = Hit;
		}
	}

	return true;
}
