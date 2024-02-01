#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utilities/UFootStepSoundManager.h"
#include "CActorSoundComponent.generated.h"

class UDataTable;
/**
 * @brief 액터의 사운드를 관리하는 컴포넌트.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCActorSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActorSoundComponent();

protected:
	virtual void BeginPlay() override;

public:
	FFootStepRowData* GetFootStepRowData(EPhysicalSurface InSurfaceType) const;

private:
	void InitFootStepData();

private:
	UPROPERTY(EditAnywhere, Category = "Sound|FootStep")
	UDataTable* FootStepDataTable;
	TArray<FFootStepRowData*> FootStepDataArray;

private:
	/**
	 * @brief DataTable을 TArray로 변환한다.
	 * @typedef T : DataTable의 RowData Struct 타입
	 * @typedef E : Array의 크기과 접근하는 Index 용도로 사용되는 Enum 타입
	 */
	template<typename T>
	void InitDataTableToArray(UDataTable* InRowDataArray, TArray<T*>& OutArray, int32(*IndexLambda)(T*))
	{
		TArray<T*> RowDataArray;
		InRowDataArray->GetAllRows<T>("", RowDataArray);

		// 최대 ENUM을 찾는다.
		int32 MaxEnum = 0;
		for (auto RowData : RowDataArray)
		{
			int32 Index = IndexLambda(RowData);
			if (MaxEnum < Index)
			{
				MaxEnum = Index;
			}
		}

		// 배열을 세팅한다.
		OutArray.SetNum(MaxEnum + 1);
		for (auto RowData : RowDataArray)
		{
			OutArray[IndexLambda(RowData)] = RowData;
		}
	}
	
};
