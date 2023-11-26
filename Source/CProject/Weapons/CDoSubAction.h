#pragma once

#include "CoreMinimal.h"
#include "CWeaponAsset.h"
#include "IDoActionDebugData.h"
#include "IExcuteAction.h"
#include "UObject/NoExportTypes.h"
#include "CDoSubAction.generated.h"

/**
 * @brief SubAction 클래스
 */
UCLASS(Abstract, NotBlueprintable)
class CPROJECT_API UCDoSubAction : public UObject
	, public IIExcuteAction
{
	GENERATED_BODY()

public:
	UCDoSubAction();

public:
	virtual void BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);

public:
	// 누르고 있는 동작을 재정의
	virtual void Pressed() {};
	// 버튼을 뗄 때의 동작을 재정의
	virtual void Released() {};

public:
	/*
	 * 기본 동작은 상속 받은 클래스에서 재정의하여 사용.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void BeginSubAction();
	virtual void BeginSubAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
	void EndSubAction();
	virtual void EndSubAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
	void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {};

public:
	virtual void Begin_Action() override {};
	virtual void End_Action() override {};
	
protected:
	UPROPERTY()
	class UCWeaponAsset* OwnerWeaponAsset;
	UPROPERTY()
	class ACharacter* OwnerCharacter;
	UPROPERTY()
	class ACAttachment* Attachment;
	UPROPERTY()
	class UCDoAction* DoAction;

	UPROPERTY()
	class UCStateComponent* StateComponent;
	UPROPERTY()
	class UCMovementComponent* MovementComponent;

/*
 * Getter / Setter
 */
public:
	virtual FString GetActionName() override { return GetName(); }
};
