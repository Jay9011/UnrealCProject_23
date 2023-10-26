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
	void BeginPlay(class ACharacter* InOwner, TArray<FEvadeData>& InData);

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
	
protected:
	TArray<FEvadeData*> EvadeDataPtr;

protected:
	UPROPERTY()
	class ACharacter* OwnerCharacter;
	UPROPERTY()
	class UCStateComponent* StateComponent;
	UPROPERTY()
	class UCEvadeComponent* EvadeComponent;
};
