#include "Character/CPlayer.h"
#include "Global.h"

#include "CAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CEvadeComponent.h"
#include "Components/InputComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyDebugger/DebuggerComponent.h"

ACPlayer::ACPlayer()
{
	SpringArm = this->CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetMesh());
	
	Camera = this->CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	Evade = this->CreateDefaultSubobject<UCEvadeComponent>("Evade");

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
	Debugger = this->CreateDefaultSubobject<UDebuggerComponent>("Debugger");
#endif
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	Movement->SetSpeed(ESpeedType::Run);
	Movement->DisableControlRotation();

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
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
	
	PlayerInputComponent->BindAction("MainWeapon", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBladeMode);
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
