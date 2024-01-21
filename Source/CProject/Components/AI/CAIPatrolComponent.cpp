#include "Components/AI/CAIPatrolComponent.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Character/CAnimInstance.h"
#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CMovementComponent.h"
#include "Components/AI/Parts/CPatrolPath.h"
#include "GameFramework/Character.h"
#include "MyDebugger/DebuggerComponent.h"

UCAIPatrolComponent::UCAIPatrolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SearchMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/Resources/Anim_Montages/Common/LookAround_Montage.LookAround_Montage'")).Object;
}

void UCAIPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	// 일단 Tick 비활성화
	PrimaryComponentTick.SetTickFunctionEnable(false);

	Owner = Cast<ACEnemy_AI>(GetOwner());

	if (Owner != nullptr)
	{
		// 초기 위치 설정
		if (PatrolPoint.Equals(FTransform::Identity))
		{
			PatrolPoint = Owner->GetTransform();
		}

		// AnimInstance 설정
		AnimInstance = Cast<UCAnimInstance>(Owner->GetMesh()->GetAnimInstance());
		if (AnimInstance != nullptr)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &UCAIPatrolComponent::EndSearchMontage);
		}

		// MovementComponent 설정
		MovementComponent = Cast<UCMovementComponent>(Owner->GetComponentByClass(UCMovementComponent::StaticClass()));
	}


#if DEBUG_AI_Patrol
	if (UDebuggerComponent* DebuggerComponent = Cast<UDebuggerComponent>(Owner->GetComponentByClass(UDebuggerComponent::StaticClass())))
		DebuggerComponent->AddCollector(this);
#endif
	
}

void UCAIPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AttachedControllerOnHead();
}

void UCAIPatrolComponent::PlaySearchMontage()
{
	if (SearchMontage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SearchMontage is nullptr"));
		return;
	}

	if (MovementComponent != nullptr)
	{
		MovementComponent->BackupControlRotation();
		MovementComponent->DisableControlRotation();
	}
	
	Owner->PlayAnimMontage(SearchMontage);
	// 몽타주가 재생되는 동안에는 NeckComponent 사용 안함
	AnimInstance->DisableNeckRotation();
	
	bPlayingSearchMontage = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
	return;
}

void UCAIPatrolComponent::EndSearchMontage(UAnimMontage* Montage, bool bInterrupted)
{
	AAIController* Controller = Owner->GetController<AAIController>();
	Controller->ClearFocus(EAIFocusPriority::Gameplay);
	
	if (Montage != SearchMontage)
		return;

	if (MovementComponent != nullptr)
	{
		MovementComponent->RestoreControlRotation();
	}

	// 몽타주가 끝나면 다시 NeckComponent 사용
	AnimInstance->EnableNeckRotation();
	
	bPlayingSearchMontage = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
}

void UCAIPatrolComponent::AttachedControllerOnHead()
{
	ACAIController* Controller = Owner->GetController<ACAIController>();

	if (Controller == nullptr)
		return;
	
	Controller->SetFocalPoint(Owner->GetHeadForwardTransform().GetTranslation());
	
#if DEBUG_AI_Patrol
	DrawDebugCircle(Owner->GetWorld(), Owner->GetHeadForwardTransform().GetTranslation(), 3.f, 6, FColor::Yellow, false, 0.1f);
#endif
}

#if DEBUG_AI_Patrol
bool UCAIPatrolComponent::IsDebugEnable()
{
	return true;
}

FDebugInfo UCAIPatrolComponent::GetDebugInfo()
{
	FDebugInfo DebugInfo;
	DebugInfo.Priority = static_cast<int32>(DEBUG_NUMS::AI_Patrol);
	DebugInfo.Data.Add({"Current ControlRotation: " + Owner->GetController()->GetControlRotation().ToString(), FColor::Cyan});
	DebugInfo.Data.Add({"bUseControlRotation Yaw: " + FString(Owner->bUseControllerRotationYaw ? "true" : "false"), FColor::Cyan});
	
	return DebugInfo;
}
#endif
