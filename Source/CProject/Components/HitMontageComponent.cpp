#include "Components/HitMontageComponent.h"

#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"

UHitMontageComponent::UHitMontageComponent()
{
}


void UHitMontageComponent::BeginPlay()
{
	Super::BeginPlay();

	if(MontagesTable == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("MontagesTable is not selected"));
		return;
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	// 데이터 테이블의 모든 데이터들을 가져온다.
	TArray<FHitMontageData*> Datas;
	MontagesTable->GetAllRows<FHitMontageData>("", Datas);

	for (FHitMontageData* Data : Datas)
	{
		MontagesData[static_cast<int32>(Data->Type)] = Data;
	}
}

void UHitMontageComponent::Play(EHitType InType, const float InPlayRate, const FName& InSectionName)
{
	FHitMontageData* data = MontagesData[static_cast<int32>(InType)];
	if (!!data)
	{
		PlayAnimMontage(InType, InPlayRate, InSectionName);
	}
	else
	{
		PlayAnimMontage(EHitType::Normal, InPlayRate, InSectionName);
	}
}

void UHitMontageComponent::PlayAnimMontage(EHitType InType, const float InPlayRate, const FName& InSectionName)
{
	CheckNull(OwnerCharacter);

	FHitMontageData* data = MontagesData[static_cast<int32>(InType)];

	if (!data)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("MontagesData is None"));

		return;
	}

	if (!data->Montage)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("Montage is None"));
		
		return;
	}

	if (InPlayRate != 0.f)
	{
		OwnerCharacter->PlayAnimMontage(data->Montage, InPlayRate, InSectionName);
	}
	else
	{
		OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
	}
}
