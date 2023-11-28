#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "HitMontageComponent.generated.h"

UENUM()
enum class EHitType : uint8
{
	Normal = 0,
	LightFront, LightLeft, LightRight, LightUp, LightDown,
	MiddleFront, MiddleLeft, MiddleRight, MiddleUp, MiddleDown,
	HeavyFront, HeavyLeft, HeavyRight, HeavyUp, HeavyDown,
	UpperNormal, UpperAndDown, FrontDownLight,
	Back,
	Max
};

USTRUCT()
struct FHitMontageData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EHitType Type;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UHitMontageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHitMontageComponent();

protected:
	virtual void BeginPlay() override;

public:
	void Play(EHitType InType, const float InPlayRate = 0.f, const FName& InSectionName = NAME_None);
	
private:
	void PlayAnimMontage(EHitType InType, const float InPlayRate = 0.f, const FName& InSectionName = NAME_None);

private:
	UPROPERTY(EditAnywhere, Category = "DataTable")
	UDataTable* MontagesTable = nullptr;
	
	FHitMontageData* MontagesData[(int32)EHitType::Max] = { nullptr };

private:
	class ACharacter* OwnerCharacter = nullptr;
};
