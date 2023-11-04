#pragma once

#include "CoreMinimal.h"
#include "IDoActionDebugData.h"
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
	
	UPROPERTY(EditAnywhere, Category = "DoAction")
	TArray<FDoActionData> DoActionDatas;

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

//Getter, Setter
public:
	FORCEINLINE class ACAttachment* GetAttachment() const { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() const { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() const { return DoAction; }
	FORCEINLINE class UCEvadeAction* GetEvadeAction() const { return EvadeAction; }
	FORCEINLINE class UCDoSubAction* GetDoSubAction() const { return DoSubAction; }
	FORCEINLINE IIExcuteAction* GetCurrentAction() const { return CurrentAction; }

	FORCEINLINE void SetCurrentAction(IIExcuteAction* InCurrentAction)
	{
		PrevAction = CurrentAction;
		CurrentAction = InCurrentAction;
	}

private:
	// 현재 예약된 액션
	IIExcuteAction* CurrentAction;
	// 이전 진행된 액션
	IIExcuteAction* PrevAction;
	
};
