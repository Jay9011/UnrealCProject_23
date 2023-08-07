#include "Weapons/Evades/CEvadeAction_Blade.h"

#include "Components/CEvadeComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"

void UCEvadeAction_Blade::OnBeginEquip()
{
	Super::OnBeginEquip();
	EvadeComponent->OnBeginEvadeDelegate.Clear();
	EvadeComponent->OnBeginEvadeDelegate.AddDynamic(this, &UCEvadeAction_Blade::BeginEvade);
	EvadeComponent->OnEndEvadeDelegate.Clear();
	EvadeComponent->OnEndEvadeDelegate.AddDynamic(this, &UCEvadeAction_Blade::EndEvade);
}

void UCEvadeAction_Blade::OnUnequip()
{
	Super::OnUnequip();
	EvadeComponent->DelegateInit();
}

void UCEvadeAction_Blade::BeginEvade()
{
	Super::BeginEvade();

	EEvadeDir Dir = EEvadeDir::FRONT;

	FVector InputDir = OwnerCharacter->GetLastMovementInputVector();
	if(!InputDir.IsNearlyZero())
	{
		InputDir.Normalize();
		// forward 벡터와 right 벡터를 구한다.
		FVector Forward = OwnerCharacter->GetActorForwardVector();
		FVector Right = OwnerCharacter->GetActorRightVector();

		Forward.Z = 0;
		Right.Z = 0;

		// 내적을 통해 어느 방향으로 회피할지 결정한다.
		float ForwardDot = FVector::DotProduct(Forward, InputDir);
		float RightDot = FVector::DotProduct(Right, InputDir);

		if (ForwardDot > 0.7) Dir = EEvadeDir::FRONT;
		if (ForwardDot < -0.7) Dir = EEvadeDir::BACK; 
		if (RightDot > 0.7) Dir = EEvadeDir::RIGHT;
		if (RightDot < -0.7) Dir = EEvadeDir::LEFT;
	}
	
	EvadeDataPtr[static_cast<int32>(Dir)]->PlayMontage(OwnerCharacter);
}

void UCEvadeAction_Blade::EndEvade()
{
	Super::EndEvade();
	StateComponent->SetIdleMode();
}
