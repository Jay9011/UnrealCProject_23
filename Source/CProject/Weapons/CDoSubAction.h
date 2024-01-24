#pragma once

#include "CoreMinimal.h"
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
	virtual void BeginPlay(class ACharacter* InOwner, class UCWeaponObject* InWeapon, class ACAttachment* InAttachment, class UCDoAction* InDoAction);

public:
	// 누르고 있는 동작을 재정의
	virtual void Pressed() { bPressed = true; }
	// 버튼을 뗄 때의 동작을 재정의
	virtual void Released() { bPressed = false; }

public:
	/*
	 * 기본 동작은 상속 받은 클래스에서 재정의하여 사용.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginSubAction();
	virtual void BeginSubAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndSubAction();
	virtual void EndSubAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
	void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {};

public:
	virtual void Begin_Action() override;
	virtual void End_Action() override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	class ACharacter* OwnerCharacter;
	UPROPERTY()
	class UCWeaponObject* Weapon;
	UPROPERTY()
	class ACAttachment* Attachment;
	UPROPERTY()
	class UCDoAction* DoAction;

	UPROPERTY()
	class UCStateComponent* StateComponent;
	UPROPERTY()
	class UCMovementComponent* MovementComponent;

	bool bPressed = false;
	
/*
 * Getter / Setter
 */
public:
	UFUNCTION(BlueprintCallable, BlueprintGetter, Category = "Character")
	FORCEINLINE class ACharacter* GetOwnerCharacter() const { return OwnerCharacter; };
	
	virtual FString GetActionName() override { return GetName(); }

	FORCEINLINE bool IsPressed() const { return bPressed; }
};
