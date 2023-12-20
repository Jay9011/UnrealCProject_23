#pragma once

#include "CoreMinimal.h"
#include "CWeaponObject.h"
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
	 * @param InOwner 소유자
	 * @param InWeapon 실제 무기 데이터
	 */
	virtual void BeginPlay
	(
		class ACharacter* InOwner,
		class UCWeaponObject* InWeapon
	);

public:
	virtual void Begin_DoAction();
	virtual void End_DoAction();

/*
 * 블루프린트에서 재정의 할 수 있는 용도의 함수
 */
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Add Event", meta = (DisplayName = "BeginPlay"))
	void BP_BeginPlay();

	UFUNCTION(BlueprintCallable, Category = "Add Event", meta = (DisplayName = "Add DoAction", ToolTip = "이 DoAction에서 사용할 DoAction을 추가합니다."))
	UCDoAction* BP_AddDoAction(TSubclassOf<UCDoAction> InDoActionClass);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action", meta = (DisplayName = "DoAction"))
	void DoAction();
	virtual void DoAction_Implementation();
	
public:
	virtual void Begin_Action() override {};
	virtual void End_Action() override {};

/*
 * 충돌체 관련 바운딩 함수
 */	
public:
	UFUNCTION()
	virtual void OnAttachmentBeginCollision()
	{
		IIExcuteAction* CurrentAction = Weapon->GetCurrentAction();
		if (CurrentAction != nullptr && CurrentAction != this)
		{
			UCDoAction* DoAction = Cast<UCDoAction>(CurrentAction);
			
			if (DoAction != nullptr)
				DoAction->OnAttachmentBeginCollision();
		}
	}
	UFUNCTION()
	virtual void OnAttachmentEndCollision()
	{
		IIExcuteAction* CurrentAction = Weapon->GetCurrentAction();
		if (CurrentAction != nullptr && CurrentAction != this)
		{
			UCDoAction* DoAction = Cast<UCDoAction>(CurrentAction);
			
			if (DoAction != nullptr)
				DoAction->OnAttachmentEndCollision();
		}
	}
	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther)
	{
		IIExcuteAction* CurrentAction = Weapon->GetCurrentAction();
		if (CurrentAction != nullptr && CurrentAction != this)
		{
			UCDoAction* DoAction = Cast<UCDoAction>(CurrentAction);
			
			if (DoAction != nullptr)
				DoAction->OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
		}
	}
	UFUNCTION()
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther)
	{
		IIExcuteAction* CurrentAction = Weapon->GetCurrentAction();
		if (CurrentAction != nullptr && CurrentAction != this)
		{
			UCDoAction* DoAction = Cast<UCDoAction>(CurrentAction);
			
			if (DoAction != nullptr)
				DoAction->OnAttachmentEndOverlap(InAttacker, InOther);
		}
	}


protected:
	bool bBeginAction;
	UPROPERTY()
	class UWorld* World;
	UPROPERTY()
	class ACharacter* OwnerCharacter;
	UPROPERTY()
	class UCWeaponObject* Weapon;
	
	UPROPERTY()
	class UCStateComponent* StateComponent;
	UPROPERTY()
	class UCMovementComponent* MovementComponent;
	
/*
 * 데이터 관리
 */
protected:
	void InitDoActionData();
	
	UPROPERTY(EditAnywhere, Category = "DataTable")
	UDataTable* ActionDataTable = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FDoActionData> DoActionDatas;
	
	/*
	 * Getter / Setter
	 */
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ACharacter* GetOwnerCharacter() const { return OwnerCharacter; }

	virtual FString GetActionName() override { return GetName(); }
};
