#include "Components/CMontagesComponent.h"
#include "Global.h"

#include "GameFramework/Character.h"

UCMontagesComponent::UCMontagesComponent()
{
}

void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	if(MontagesTable == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("MontagesTable is not selected"));
		return;
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	// 데이터 테이블의 모든 데이터들을 가져온다.
	TArray<FMontagesData*> Datas;
	MontagesTable->GetAllRows<FMontagesData>("", Datas);

	for(int32 i = 0; i < static_cast<int32>(EStateType::Max); i++)
	{
		for(FMontagesData* Data : Datas)
		{
			if(static_cast<EStateType>(i) == Data->Type)
			{
				MontagesData[i] = Data;
				
				continue;
			}
		}// for(FMontagesData* Data : Datas)
	}// for(int32 i = 0; i < static_cast<int32>(EStateType::Max); i++)
	
}

void UCMontagesComponent::Idle()
{
	PlayAnimMontage(EStateType::Idle);
}

void UCMontagesComponent::Dead()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InType)
{
	CheckNull(OwnerCharacter);

	FMontagesData* data = MontagesData[static_cast<int32>(InType)];

	if (data == nullptr)
		return;

	if (data->Montage == nullptr)
		return;

	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
}
