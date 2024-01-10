#include "Weapons/CtrlActions/CGuardAction.h"

#include "Components/CGuardComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CGuardMontageAsset.h"
#include "Weapons/CWeaponStructures.h"

UCGuardAction::UCGuardAction()
{
}

void UCGuardAction::BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InWeapon, InAttachment);
	
	
	StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	MovementComponent = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));

	GuardComponent = Cast<UCGuardComponent>(InOwner->GetComponentByClass(UCGuardComponent::StaticClass()));
	if (GuardComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GuardComponent is nullptr"));
		return;
	}

	AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

	/* AnimInstance에 몽타주의 MontageNotify 델리게이트를 등록 */
	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UCGuardAction::OnPlayMontageNotifyBegin);
}

void UCGuardAction::Pressed()
{
	Super::Pressed();
	CheckNull(GuardComponent);
	CheckNull(AnimInstance);

	if (CanBlocking())
	{
		FGuardMontageData MontageData;
		GuardMontageAsset->PlayStartBlockMontage(AnimInstance, MontageData);
		MovementProcess(MontageData.CanMove, MontageData.FixedCamera);
	}
}

void UCGuardAction::Released()
{
	Super::Released();
	CheckNull(GuardComponent);
	CheckNull(AnimInstance);
	CheckNull(GuardMontageAsset);

	// 다른 상태 변화로 인해 가드가 풀린게 아니라면, 정상적인 가드 해제 동작을 수행한다.
	if (!GuardComponent->IsUnGuard())
	{
		// 만약, BlockCancel이 가능한 타이밍이라면 바로 EndBlockingMode를 호출한다.
		if (BlockCancel)
		{
			EndBlockingMode();
			FGuardMontageData MontageData;
			GuardMontageAsset->PlayEndBlockMontage(AnimInstance, MontageData);
			MovementProcess(MontageData.CanMove, MontageData.FixedCamera);
		}
		// 그렇지 않다면, BlockCancel이 가능한 타이밍이 될 때까지 대기한다.
		else
		{
			ReserveBlockCancel = true;
		}
	}
}

void UCGuardAction::OnBeginEquip()
{
	Super::OnBeginEquip();

	CheckNull(GuardComponent);
	
	if (GuardComponent != nullptr)
	{
		GuardComponent->OnEvaluateBlocking.AddDynamic(this, &UCGuardAction::OnEvaluateBlocking);
		GuardComponent->OnEvaluateParrying.AddDynamic(this, &UCGuardAction::OnEvaluateParrying);
	}

	if (StateComponent != nullptr)
	{
		StateComponent->OnStateTypeChanged.AddDynamic(this, &UCGuardAction::OnStateTypeChanged);
	}
}

void UCGuardAction::OnUnequip()
{
	Super::OnUnequip();

	CheckNull(GuardComponent);
	
	if (GuardComponent != nullptr)
	{
		GuardComponent->OnEvaluateBlocking.RemoveDynamic(this, &UCGuardAction::OnEvaluateBlocking);
		GuardComponent->OnEvaluateParrying.RemoveDynamic(this, &UCGuardAction::OnEvaluateParrying);
	}

	if (StateComponent != nullptr)
	{
		StateComponent->OnStateTypeChanged.RemoveDynamic(this, &UCGuardAction::OnStateTypeChanged);
	}
}

void UCGuardAction::SetBlockingMode()
{
	if (GuardComponent->IsUnGuard())
	{
		MovementComponent->OffCrouch();
	
		GuardComponent->SetBlockingMode();
		SetParryingMode();
	}
}

void UCGuardAction::EndBlockingMode()
{
	// 현재 패링 상태라면 패링과 관련된 타이머를 모두 종료한다.
	if (GuardComponent->IsParrying())
	{
		EndParryingMode();
	}

	if (GuardComponent->IsBlocking())
	{
		GuardComponent->SetUnGuard();
	}

	BlockCancel = false;
	ReserveBlockCancel = false;

	MovementProcess();
}

void UCGuardAction::OnEvaluateBlocking(FDamagedData& DamagedData)
{
	bool GuardBreakable = DamagedData.Event->HitData->bGuardBreakable;
	if (GuardBreakable)
	{
		GuardBreak(DamagedData);
	}
	else
	{
		BlockingSuccess(DamagedData);
	}
}

void UCGuardAction::BlockingSuccess(FDamagedData& DamagedData)
{
	// 만약, 몽타주가 BlendIn 되기 전이라면, 몽타주 재생을 처리하지 않는다.
	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	if (CurrentMontage)
	{
		float CurrentPosition = AnimInstance->Montage_GetPosition(CurrentMontage);
		float BlendInTime = CurrentMontage->BlendIn.GetBlendTime();
		if (CurrentPosition < BlendInTime)
		{
			return;
		}
	}
	
	FGuardMontageData MontageData;
	GuardMontageAsset->PlaySuccessBlockMontage(AnimInstance, MontageData);
	MovementProcess(MontageData.CanMove, MontageData.FixedCamera);
	
	// 가드 성공 이후 돌아오는 Notify를 수신할 때 까지 BlockCancel을 할 수 없다.
	BlockCancel = false;
}

void UCGuardAction::GuardBreak(FDamagedData& DamagedData)
{
	// 가드 브레이크는 기본적으로 가드를 해제한다.
	EndBlockingMode();
	
	FGuardMontageData MontageData;
	GuardMontageAsset->PlayBreakBlockMontage(AnimInstance, MontageData);
	MovementProcess(MontageData.CanMove, MontageData.FixedCamera);

	// 가드 브레이크 이후 돌아오는 Notify를 수신할 때 까지 무방비 상태가 된다.
	if (StateComponent != nullptr)
	{
		StateComponent->SetUnprotectedMode();
	}
}

void UCGuardAction::SetParryingMode()
{
	FTimerManager& TimerManager = OwnerCharacter->GetWorld()->GetTimerManager();
	if (FMath::IsNearlyZero(ParryingDelay))
	{
		GuardComponent->SetParryingMode();
	}
	else
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([&]()
		{
			GuardComponent->SetParryingMode();
		});
		TimerManager.SetTimer(ParryingTimerHandle, TimerDelegate, ParryingDelay, false);
	}

	FTimerDelegate EndTimerDelegate;
	EndTimerDelegate.BindLambda([&]()
	{
		GuardComponent->SetBlockingMode();
	});
	TimerManager.SetTimer(ParryingEndTimerHandle, EndTimerDelegate, ParryingDuration, false);
}

void UCGuardAction::EndParryingMode()
{
	// 만약 패링 관련 타이머가 남아있다면 모두 종료한다.
	ClearParryingTimer();

	GuardComponent->SetBlockingMode();
}

void UCGuardAction::OnEvaluateParrying(FDamagedData& DamagedData)
{
	ParryingSuccess(DamagedData);
}

void UCGuardAction::ParryingSuccess(FDamagedData& DamagedData)
{
	
}

void UCGuardAction::ClearParryingTimer()
{
	FTimerManager& TimerManager = OwnerCharacter->GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(ParryingTimerHandle);
	TimerManager.ClearTimer(ParryingEndTimerHandle);
}

bool UCGuardAction::CanBlocking() const
{
	CheckFalseResult(StateComponent->IsIdleMode(), false);

	return GuardComponent->IsUnGuard();
}

void UCGuardAction::OnPlayMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (GuardMontageAsset->GetBeginBlockNotifyName() == NotifyName)
	{
		SetBlockingMode();
	}
	else if (GuardMontageAsset->GetEndBlockNotifyName() == NotifyName)
	{
		EndBlockingMode();
	}
	// GuardSuccess 이후 Loop 로 돌아오는 동작
	else if (GuardMontageAsset->GetReturnToLoopNotifyName() == NotifyName)
	{
		BlockCancel = true;
		// 만약, 버튼을 뗀 상태라면 바로 EndBlockingMode를 호출한다.
		if (ReserveBlockCancel)
		{
			EndBlockingMode();
			FGuardMontageData MontageData;
			GuardMontageAsset->PlayEndBlockMontage(AnimInstance, MontageData);
			MovementProcess(MontageData.CanMove, MontageData.FixedCamera);
		}
		else if (GuardComponent->IsBlocking())
		{
			FGuardMontageData MontageData;
			GuardMontageAsset->PlayLoopBlockMontage(AnimInstance, MontageData);
			MovementProcess(MontageData.CanMove, MontageData.FixedCamera);
		}
	}
	else if (GuardMontageAsset->GetCheckBlockCancelNotifyName() == NotifyName)
	{
		BlockCancel = true;
		// 만약, 버튼을 뗀 상태라면 바로 EndBlockingMode를 호출한다.
		if (ReserveBlockCancel)
		{
			EndBlockingMode();
			FGuardMontageData MontageData;
			GuardMontageAsset->PlayEndBlockMontage(AnimInstance, MontageData);
			MovementProcess(MontageData.CanMove, MontageData.FixedCamera);
		}
	}
}

void UCGuardAction::OnStateTypeChanged(EStateType InPrevStateType, EStateType InNewStateType)
{
	switch (InPrevStateType)
	{
	case EStateType::Unprotected:
		{
			if (InNewStateType == EStateType::Idle)
			{
				// 움직임 되돌리기
				MovementProcess();
			}
		}
		break;
		
	default: ;
	}
}

void UCGuardAction::MovementProcess(bool CanMove, bool FixedCamera)
{
	if (CanMove)
	{
		MovementComponent->Move();
	}
	else
	{
		MovementComponent->Stop();
	}

	if (FixedCamera)
	{
		MovementComponent->EnableFixedCamera();
	}
	else
	{
		MovementComponent->DisableFixedCamera();
	}
}
