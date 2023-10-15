#include "Character/CAnimInstance.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	//WeaponComponent를 찾아서 WeaponTypeChanged 델리게이트에 OnWeaponTypeChanged를 바인딩해 무기 변경을 감지한다.
	Weapon = Cast<UCWeaponComponent>(OwnerCharacter->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if(!!Weapon)
	{
		//무기 변경을 감지하면 OnWeaponTypeChanged 함수를 호출한다.
		Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
	}
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

	// Use Control Rotation을 사용하면 컨트롤러의 회전값을 사용하고, 아니면 캐릭터의 속도를 사용한다.
	if(OwnerCharacter->bUseControllerRotationYaw)
	{
		const FRotator Rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
		const FRotator Rotator2 = OwnerCharacter->GetControlRotation();
		const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(Rotator, Rotator2);
		PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, Delta, DeltaSeconds, 25);
		Direction = PrevRotation.Yaw;
	}

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}