#include "Character/CAnimInstance.h"
#include "Global.h"

#include "GameFramework/Character.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);
	
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

	
	const FRotator Rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	const FRotator Rotator2 = OwnerCharacter->GetControlRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(Rotator, Rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, Delta, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);
}
