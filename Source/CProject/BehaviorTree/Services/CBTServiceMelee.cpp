#include "BehaviorTree/Services/CBTServiceMelee.h"

#include "CGameMode.h"
#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CMovementComponent.h"
#include "Components/AI/CAIBehaviorComponent.h"
#include "GameMode/CBattleSystemComponent.h"

UCBTServiceMelee::UCBTServiceMelee()
{
	NodeName = "Melee";

	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UCBTServiceMelee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* OwnerController = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(OwnerController->GetPawn());
	
	UCStateComponent* State = Cast<UCStateComponent>(Enemy_AI->GetComponentByClass(UCStateComponent::StaticClass()));
	UCAIBehaviorComponent* Behavior = Cast<UCAIBehaviorComponent>(Enemy_AI->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	ACGameMode* GameMode = Cast<ACGameMode>(GetWorld()->GetAuthGameMode());
	UCBattleSystemComponent* BattleSystem = GameMode->GetBattleSystemComponent();

	if (State->IsDeadMode())
	{
		Behavior->SetDeadMode();
		BattleSystem->UnregisterAIController(OwnerController);

		return;
	}

	if (State->IsHittedMode())
	{
		Behavior->SetHittedMode();

		return;
	}

	ACharacter* Target = Behavior->GetTarget();
	if (Target == nullptr)
	{
		Behavior->SetPatrolMode();
		BattleSystem->UnregisterAIController(OwnerController);

		return;
	}

	BattleSystem->UpdateAttackingAI();
	
	float Distance = Enemy_AI->GetDistanceTo(Target);
	if (Distance < MeleeRange)
	{
		Behavior->SetActionMode();
		BattleSystem->RegisterAIController(OwnerController);

		return;
	}

	if (Behavior->IsAction() && Distance < MaxMeleeRange)
	{
		return;
	}

	Behavior->SetApproachMode();
}
