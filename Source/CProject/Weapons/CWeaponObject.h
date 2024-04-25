#pragma once

#include "CoreMinimal.h"
#include "Weapons/IExcuteAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CWeaponObject.generated.h"

class UCAddOnComponent;
/**
 * @brief Weapon 인스턴스를 관리하는 UObject
 */
UCLASS()
class CPROJECT_API UCWeaponObject : public UObject
{
	GENERATED_BODY()
private:
	friend class UCWeaponAsset;

public:
	void RegisterAddOnComponent(UCAddOnComponent* InAddOnComponent);
	
	template <typename T>
	T* GetAddOnComponent()
	{
		for (auto AddOnComponent : AddOnComponents)
		{
			if (T* Casted = Cast<T>(AddOnComponent))
				return Casted;
		}
		return nullptr;
	}
	
/*
 * Action 예약 관련
 */
public:
	void UpdateActions();
	
/*
 * 실제 사용할 인스턴스
 */
private:
	UPROPERTY()
	class ACAttachment* Attachment;
	UPROPERTY()
	class UCEquipment* Equipment;
	UPROPERTY()
	class UCDoAction* DoAction;
	UPROPERTY()
	class UCDoSubAction* DoSubAction;
	UPROPERTY()
	class UCEvadeAction* EvadeAction;
	UPROPERTY()
	class UCCtrlAction* CtrlAction;

private:
	UPROPERTY()
	TArray<UCAddOnComponent*> AddOnComponents;

	
public:
	FORCEINLINE IIExcuteAction* GetCurrentAction() const { return CurrentAction; }
	FORCEINLINE IIExcuteAction* GetReservedAction() const { return ReservedAction; }

	FORCEINLINE void SetCurrentAction(IIExcuteAction* InCurrentAction)
	{
		if(!!CurrentAction)
			PrevAction = CurrentAction;
		
		CurrentAction = InCurrentAction;
	}
	FORCEINLINE void ReserveAction(IIExcuteAction* InReserveAction)
	{
		// CurrentAction이 없다면 예약된 Action을 바로 실행
		if(!!CurrentAction)
			ReservedAction = InReserveAction;
		else
			SetCurrentAction(InReserveAction);
	}
	
private:
	IIExcuteAction* PrevAction = nullptr;		// 이전 Action
	IIExcuteAction* CurrentAction = nullptr;	// 현재 Action
	IIExcuteAction* ReservedAction = nullptr;	// 예약된 Action

/*
 *	Getter, Setter
 */
public:
	FORCEINLINE class ACAttachment* GetAttachment() const { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() const { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() const { return DoAction; }
	FORCEINLINE class UCEvadeAction* GetEvadeAction() const { return EvadeAction; }
	FORCEINLINE class UCDoSubAction* GetDoSubAction() const { return DoSubAction; }
	FORCEINLINE class UCCtrlAction* GetCtrlAction() const { return CtrlAction; }

};
