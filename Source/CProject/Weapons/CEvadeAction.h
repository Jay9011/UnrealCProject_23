#pragma once

#include "CoreMinimal.h"
#include "Components/IEvadeAction.h"
#include "UObject/NoExportTypes.h"
#include "CEvadeAction.generated.h"

/**
 * @brief 무기 회피 클래스
 */
UCLASS(Abstract, NotBlueprintable)
class CPROJECT_API UCEvadeAction : public UObject
{
	GENERATED_BODY()

public:
	virtual void BeginPlay(class ACharacter* InOwner, TArray<FEvadeData>& InData);

public:
	UFUNCTION()
	virtual void OnBeginEquip();
	UFUNCTION()
	virtual void OnUnequip();

public:
	UFUNCTION()
	virtual void BeginEvade();
	UFUNCTION()
	virtual void EndEvade();

private:
	// 상태 변경시 호출되는 함수
	UFUNCTION()
	virtual void OnBeforeStateChange(EStateType InPrevState, EStateType InNewState);
	
protected:
	TArray<FEvadeData*> EvadeDataPtr;

protected:
	UPROPERTY()
	class ACharacter* OwnerCharacter;
	UPROPERTY()
	class UCStateComponent* StateComponent;
	UPROPERTY()
	class UCMovementComponent* MovementComponent;
	UPROPERTY()
	class UCEvadeComponent* EvadeComponent;
};
