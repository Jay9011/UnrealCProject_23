#include "Components/AI/CMovementComponent_AI.h"

#include "Character/AI/CAIController.h"
#include "GameFramework/Character.h"

void UCMovementComponent_AI::BeginPlay()
{
	Super::BeginPlay();

	OwnerAIController = OwnerCharacter->GetController<ACAIController>();
}

void UCMovementComponent_AI::Move()
{
	Super::Move();

}

void UCMovementComponent_AI::Stop()
{
	Super::Stop();
	
	if (OwnerAIController.IsValid())
	{
		OwnerAIController->StopMovement();
	}
}
