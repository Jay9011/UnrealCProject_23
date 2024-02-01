#include "Components/CActorSoundComponent.h"

#include "Engine/DataTable.h"
#include "Utilities/CheckMacros.h"


UCActorSoundComponent::UCActorSoundComponent()
{
}

void UCActorSoundComponent::BeginPlay()
{
	Super::BeginPlay();

	InitFootStepData();
}

FFootStepRowData* UCActorSoundComponent::GetFootStepRowData(EPhysicalSurface InSurfaceType) const
{
	if (!FootStepDataArray.IsValidIndex(InSurfaceType))
	{
		return nullptr;
	}

	FFootStepRowData* Result = FootStepDataArray[static_cast<int32>(InSurfaceType)];
	if (Result == nullptr || Result->Sound == nullptr)
	{
		return nullptr;
	}

	return Result;
}

void UCActorSoundComponent::InitFootStepData()
{
	CheckNull(FootStepDataTable);

	if (FootStepDataTable->RowStruct != FFootStepRowData::StaticStruct())
	{
		UE_LOG(LogTemp, Error, TEXT("DataTable RowStruct is not equal to FFootStepRowData"));
		return;
	}

	InitDataTableToArray<FFootStepRowData>(FootStepDataTable, FootStepDataArray,
		[](FFootStepRowData* InRowData){return static_cast<int32>(InRowData->SurfaceType);}
	);
}
