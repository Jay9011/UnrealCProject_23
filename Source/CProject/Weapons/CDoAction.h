#pragma once

#include "CoreMinimal.h"
#include "CWeaponStructures.h"
#include "IDoActionDebugData.h"
#include "UObject/NoExportTypes.h"
#include "CDoAction.generated.h"

/**
 * @brief 액션 버튼을 눌렀을 때 관련 클래스
 */
UCLASS(Abstract, NotBlueprintable)
class CPROJECT_API UCDoAction : public UObject
	, public IIDoActionDebugData
{
	GENERATED_BODY()

public:
	UCDoAction();
	
	/**
	 * @brief 액션 관련 데이터 셋 배열을 받아서 초기화 (이때, 무기 외형과 장착 동작 관련 오브젝트도 받아서 초기화 한다.)
	 * 
	 * @param InAttachment 외형 관련 액터 
	 * @param InEquipment 장착 동작 관련 오브젝트
	 * @param InOwner 소유자
	 * @param InDoActionDatas 액션 관련 데이터 셋 배열 
	 */
	virtual void BeginPlay
	(
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACharacter* InOwner,
		const TArray<FDoActionData>& InDoActionDatas
	);
	
public:
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();
	
protected:
	bool bBeginAction;

	class ACharacter* OwnerCharacter;
	class UWorld* World;

	class UCStateComponent* StateComponent;
	class UCMovementComponent* MovementComponent;

	TArray<FDoActionData> DoActionDatas;
};
