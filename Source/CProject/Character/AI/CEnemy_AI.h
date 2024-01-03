#pragma once

#include "CoreMinimal.h"
#include "Character/CEnemy.h"
#include "DebugHeader.h"
#include "CEnemy_AI.generated.h"

class ACPatrolPath;
class UWidgetComponent;
class UBehaviorTree;
class UCAIBehaviorComponent;

/**
 * @brief AI Enemy Base
 */
UCLASS()
class CPROJECT_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()
public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void Hitted() override;
	virtual void End_Hitted() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	UCAIBehaviorComponent* BehaviorComponent = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
	ACPatrolPath* PatrolPath = nullptr;
	
/*
 * Getter
 */
public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	FORCEINLINE ACPatrolPath* GetPatrolPath() const { return PatrolPath; }

/*
 * Debug
 */
#if WITH_EDITOR
private:
	void UpdateLabelRenderScale();

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI|Label")
	float LabelViewAmount = 3000.f;

	UPROPERTY(VisibleDefaultsOnly)
	UWidgetComponent* LabelWidget = nullptr;
#endif
	
};
