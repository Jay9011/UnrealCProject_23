#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Components/IEvadeAction.h"
#include "Weapons/IExcuteAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"
/**
 * @brief Weapon Data를 관리하는 DataAsset
 */
UCLASS()
class CPROJECT_API UCWeaponAsset : public UDataAsset
{
private:
	GENERATED_BODY()

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);
	
private:
	/*
	 * 사용 할 클래스 선택용
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACAttachment> AttachmentClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoSubAction> DoSubActionClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEvadeAction> EvadeActionClass;
	
private:
	//Datas
	UPROPERTY(EditAnywhere, Category = "Equipment")
	FEquipmentData EquipmentData;
	
	UPROPERTY(EditAnywhere, Category = "Evade")
	TArray<FEvadeData> EvadeData;
	
private:
	/*
	 * 실제 사용할 인스턴스
	 */
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

/*
 * Action 예약 관련
 */
public:
	void UpdateActions();
	
private:
	IIExcuteAction* PrevAction;		// 이전 Action
	IIExcuteAction* CurrentAction;	// 현재 Action
	IIExcuteAction* ReservedAction;	// 예약된 Action
	
//Getter, Setter
public:
	FORCEINLINE class ACAttachment* GetAttachment() const { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() const { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() const { return DoAction; }
	FORCEINLINE class UCEvadeAction* GetEvadeAction() const { return EvadeAction; }
	FORCEINLINE class UCDoSubAction* GetDoSubAction() const { return DoSubAction; }
	
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
};
