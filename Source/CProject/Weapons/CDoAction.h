#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeaponAsset.h"
#include "CWeaponStructures.h"
#include "IDoActionDebugData.h"
#include "IExcuteAction.h"
#include "UObject/NoExportTypes.h"
#include "CDoAction.generated.h"

/**
 * @brief 액션 버튼을 눌렀을 때 관련 클래스
 */
UCLASS(Abstract, NotBlueprintable)
class CPROJECT_API UCDoAction : public UObject
	, public IIExcuteAction
{
	GENERATED_BODY()

public:
	UCDoAction();
	
	/**
	 * @brief 액션 관련 데이터 셋 배열을 받아서 초기화 (이때, 무기 외형과 장착 동작 관련 오브젝트도 받아서 초기화 한다.)
	 *
	 * @param InOwnerWeaponAsset 무기 에셋
	 * @param InAttachment 외형 관련 액터 
	 * @param InEquipment 장착 동작 관련 오브젝트
	 * @param InOwner 소유자
	 * @param InDoActionDatas 액션 관련 데이터 셋 배열 
	 */
	virtual void BeginPlay
	(
		class UCWeaponAsset* InOwnerWeaponAsset,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACharacter* InOwner, const TArray<FDoActionData>& InDoActionDatas
	);
	
public:
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

public:
	virtual void Begin_Action() override {};
	virtual void End_Action() override {};

/*
 * 충돌체 관련 바운딩 함수
 */	
public:
	UFUNCTION()
	virtual void OnAttachmentBeginCollision(){}
	UFUNCTION()
	virtual void OnAttachmentEndCollision(){}
	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther){}
	UFUNCTION()
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther){}
	
protected:
	bool bBeginAction;

	UPROPERTY()
	class UCWeaponAsset* OwnerWeaponAsset;
	UPROPERTY()
	class ACharacter* OwnerCharacter;
	UPROPERTY()
	class UWorld* World;

	UPROPERTY()
	class UCStateComponent* StateComponent;
	UPROPERTY()
	class UCMovementComponent* MovementComponent;

	TArray<FDoActionData> DoActionDatas;
};
