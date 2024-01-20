#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ActorComponent.h"
#include "CBattleSystemComponent.generated.h"

class ACAIController;

USTRUCT()
struct FDistanceAIController
{
	GENERATED_BODY()
public:
	float Distance = 0.f;
	TWeakObjectPtr<ACAIController> AIController = nullptr;

	bool operator<(const FDistanceAIController& InOther) const
	{
		return Distance < InOther.Distance;
	}
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCBattleSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCBattleSystemComponent();

protected:
	virtual void BeginPlay() override;

public:
	void RegisterAIController(ACAIController* InAIController);
	void UnregisterAIController(ACAIController* InAIController);
	
	void UpdateAttackingAI();

private:
	bool IsInvalidatedAI(TWeakObjectPtr<ACAIController>& InAIController); 
	void CleanList();
	void CleanList(TArray<TWeakObjectPtr<ACAIController>>& InRemoveAIController);
	bool FindClosestAI(TWeakObjectPtr<ACAIController>& OutClosestAIController);
	void SortAIControllers();
	
private:
	TArray<FDistanceAIController> DistanceAIControllers;
	TSet<TWeakObjectPtr<ACAIController>> RegisteredAIControllers;

	int32 AttackingAI = 0;
	
	UPROPERTY(EditAnywhere, Category = "Battle|AI")
	int32 MaxAttackingAI = 1;
	
	UPROPERTY(EditAnywhere, Category = "Key")
	FName CanAttackKey = "CanAttack";
};
