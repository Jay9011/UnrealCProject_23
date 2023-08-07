#pragma once

#include "CoreMinimal.h"
#include "IEvadeAction.h"
#include "Components/ActorComponent.h"
#include "CEvadeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBeginEvadeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndEvadeDelegate);

/**
 * @brief 회피 컴포넌트
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCEvadeComponent
	: public UActorComponent
{
	GENERATED_BODY()

public:	
	UCEvadeComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnEvade();
	UFUNCTION()
	void OnBeginEvade();
	UFUNCTION()
	void OnEndEvade();

public:
	void DelegateInit();
	
public:
	FBeginEvadeDelegate OnBeginEvadeDelegate;
	FEndEvadeDelegate OnEndEvadeDelegate;

private:
	// 회피 데이터
	UPROPERTY(EditAnywhere, Category = "Evade")
	TArray<FEvadeData> EvadeData;
	TArray<FEvadeData*> EvadeDataPtr;

private:
	UPROPERTY()
	class ACBaseCharacter* Owner;
	UPROPERTY()
	class UCMovementComponent* Movement;
	UPROPERTY()
	class UCStateComponent* State;
};
