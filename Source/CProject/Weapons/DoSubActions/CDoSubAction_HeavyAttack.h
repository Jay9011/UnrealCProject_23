#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "Weapons/IComboState.h"
#include "CDoSubAction_HeavyAttack.generated.h"

/**
 * @brief 강공격 SubAction
 */
UCLASS(Blueprintable)
class CPROJECT_API UCDoSubAction_HeavyAttack : public UCDoSubAction
	, public IIComboState 
{
	GENERATED_BODY()

public:
	virtual void BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment, UCDoAction* InDoAction) override;
	
public:
	virtual void Pressed() override;

	virtual void BeginSubAction_Implementation() override;
	virtual void EndSubAction_Implementation() override;

public:
	virtual void Begin_Action() override;
	virtual void End_Action() override;

public:
	virtual void ChangingProcess() override;

	/*
	 * 충돌체 관련 바운딩 함수
	 */	
	public:
	UFUNCTION(BlueprintNativeEvent)
	void OnAttachmentBeginCollision();
	virtual void OnAttachmentBeginCollision_Implementation(){}

	UFUNCTION(BlueprintNativeEvent)
	void OnAttachmentEndCollision();
	virtual void OnAttachmentEndCollision_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, class UPrimitiveComponent* OverlappedComponent, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);
	virtual void OnAttachmentBeginOverlap_Implementation(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, class UPrimitiveComponent* OverlappedComponent, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther, class UPrimitiveComponent* OverlappedComponent, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void OnAttachmentEndOverlap_Implementation(class ACharacter* InAttacker, class ACharacter* InOther, class UPrimitiveComponent* OverlappedComponent, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){}
	
/*
 * 데이터 관리
 */
protected:
	void InitDoActionData();
	
	UPROPERTY(EditAnywhere, Category = "DataTable")
	UDataTable* HeavyAttackDataTable = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<FDoActionData> HeavyAttackDatas;

protected:
	// CDoAction_Combo 인 경우 해당 index를 사용해야 하므로 다운캐스팅 해서 사용한다.
	UPROPERTY()
	class UCDoAction_Combo* DoAction_Combo;

	// HeavyAttack의 Combo 시스템은 DoActionCombo에서 가져온다.
	UPROPERTY(BlueprintReadOnly)
	class UCComboState* ComboState;

	
private:
	// 현재 데미지를 입은 캐릭터들
	TArray<class ACharacter*> DamagedCharacters;

/*
 * Getter / Setter
 */
public:
	UFUNCTION(BlueprintCallable, BlueprintGetter, Category = "Combo")
	virtual UCComboState* GetComboState() override { return ComboState; }
	
};
