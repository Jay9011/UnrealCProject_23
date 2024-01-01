#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "CGuardMontageAsset.generated.h"

USTRUCT()
struct FGuardMontageData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	float PlayRate = 1;

	UPROPERTY(EditAnywhere, Category = "Movement")
	bool CanMove = false;

	UPROPERTY(EditAnywhere, Category = "Movement")
	bool FixedCamera = false;
};

/**
 * @brief 가드 동작을 담는 데이터 에셋
 */
UCLASS(Abstract, NotBlueprintable)
class CPROJECT_API UCGuardMontageAsset : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Guard")
	FName BeginBlockNotifyName = "BeginBlock";
	UPROPERTY(EditDefaultsOnly, Category = "Guard")
	FName CheckBlockCancelNotifyName = "BlockCancel";
	UPROPERTY(EditDefaultsOnly, Category = "Guard")
	FName LoopSectionName = "Loop";
	
	UPROPERTY(EditDefaultsOnly, Category = "Guard Success")
	FName ReturnToLoopNotifyName = "EndSuccess";

	UPROPERTY(EditDefaultsOnly, Category = "Guard End")
	FName EndBlockNotifyName = "EndBlock";
	
protected:
	void MovementProcess(UAnimInstance* InAnimInstance, const FGuardMontageData& MontageData);
	
/*
 * Getter
 */
public:
	FName GetBeginBlockNotifyName() const { return BeginBlockNotifyName; }
	FName GetLoopSectionName() const { return LoopSectionName; }
	FName GetReturnToLoopNotifyName() const { return ReturnToLoopNotifyName; }
	FName GetEndBlockNotifyName() const { return EndBlockNotifyName; }
	FName GetCheckBlockCancelNotifyName() const { return CheckBlockCancelNotifyName; }
	
/*
 * 상속해서 사용할 것
 */
public:
	virtual void PlayStartBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData) PURE_VIRTUAL(UCGuardMontageAsset::PlayStartBlockMontage, return;)
	virtual void PlayLoopBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData) PURE_VIRTUAL(UCGuardMontageAsset::PlayLoopBlockMontage, return;)
	virtual void PlaySuccessBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData) PURE_VIRTUAL(UCGuardMontageAsset::PlaySuccessBlockMontage, return;)
	virtual void PlayBreakBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData) PURE_VIRTUAL(UCGuardMontageAsset::PlayBreakBlockMontage, return;)
	virtual void PlayEndBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData) PURE_VIRTUAL(UCGuardMontageAsset::PlayEndBlockMontage, return;)

};
