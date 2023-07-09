#pragma once

#include "CoreMinimal.h"
#include "IEvadeAction.h"
#include "Components/ActorComponent.h"
#include "CEvadeComponent.generated.h"

/**
 * @brief 회피 컴포넌트
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCEvadeComponent
	: public UActorComponent
	, public IIEvadeAction
{
	GENERATED_BODY()

public:	
	UCEvadeComponent();

protected:
	virtual void BeginPlay() override;
	
/*
 * (IIEvadeAction) 비무장 상태의 기본 회피 동작을 정의
 */	
public:
	virtual void OnEvade() override;
	virtual void Evade_Implementation() override;
	virtual void EndEvade_Implementation() override;

private:
	UPROPERTY(EditAnywhere, Category = "Evade")
	TArray<FEvadeData> EvadeData;
	TArray<FEvadeData*> EvadeDataPtr;

private:
	class ACBaseCharacter* Owner;
	class UCMovementComponent* Movement;
	class UCStateComponent* State;
};
