#include "Character/CPlayer.h"
#include "Global.h"

#include "CAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CAirComponent.h"
#include "Components/CEvadeComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/InputComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyDebugger/DebuggerComponent.h"
#include "Utilities/UDirectionalUtilities.h"

ACPlayer::ACPlayer()
{
#if DEBUG_DEFAULT_INFO
	PrimaryActorTick.bCanEverTick = true;
#endif
	
	SpringArm = this->CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetMesh());
	
	Camera = this->CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	Movement = this->CreateDefaultSubobject<UCMovementComponent>("Movement");
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

	Movement->SetSpeed(ESpeedType::Run);
	Movement->DisableControlRotation();

#if DEBUG_DEFAULT_INFO
	Debugger->AddCollector(this);
#endif
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMovementComponent::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", Movement, &UCMovementComponent::OnVerticalLook);

	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnWalk);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);
	
	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, Evade, &UCEvadeComponent::OnEvade);
	
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);
	
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SubAction_Pressed);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, Weapon, &UCWeaponComponent::SubAction_Released);
	
	PlayerInputComponent->BindAction("MainWeapon", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetMainWeaponMode);
}

void ACPlayer::Falling()
{
	Super::Falling();

	CheckNull(Air);

	// 공중 상태 변화 전인 경우
	if (!Air->IsAir())
	{
		Air->SetFallMode();
	}
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Evade:
		Evade->OnBeginEvadeDelegate.Broadcast();
		break;
	}
}

void ACPlayer::End_Evade()
{
	Evade->OnEndEvadeDelegate.Broadcast();
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
	if(!!Movement)
		Info.Data.Add({"Standing Type : " + StaticEnum<EStandingType>()->GetNameStringByIndex(static_cast<int32>(Movement->GetStandingType())), FColor::White});
	
	// Air 상태 출력
	if (!!Air)
		Info.Data.Add({"Air Type : " + StaticEnum<EAirState>()->GetNameStringByIndex(static_cast<int32>(Air->GetAirState())), FColor::White});
	
	return Info;
}
#endif
