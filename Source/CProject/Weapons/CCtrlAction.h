#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCtrlAction.generated.h"

/**
 * @brief Ctrl 키를 누르고 있는 동안의 동작을 정의하는 클래스
 */
UCLASS(Abstract, NotBlueprintable)
class CPROJECT_API UCCtrlAction : public UObject
{
	GENERATED_BODY()
public:
	UCCtrlAction();

public:
	virtual void BeginPlay(class ACharacter* InOwner, class UCWeaponObject* InWeapon, class ACAttachment* InAttachment);

public:
	virtual void Pressed() {};
	virtual void Released() {};

public:
	UFUNCTION()
	virtual void OnBeginEquip() {};
	UFUNCTION()
	virtual void OnUnequip() {};
	
protected:
	UPROPERTY()
	class ACharacter* OwnerCharacter;
	UPROPERTY()
	class UCWeaponObject* Weapon;
	UPROPERTY()
	class ACAttachment* Attachment;
};
