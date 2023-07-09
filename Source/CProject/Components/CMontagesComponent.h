#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CStateComponent.h"
#include "Engine/DataTable.h"
#include "CMontagesComponent.generated.h"

USTRUCT()
struct FMontagesData
	: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EStateType Type;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "DataTable")
	UDataTable* MontagesTable;

public:	
	UCMontagesComponent();

protected:
	virtual void BeginPlay() override;

private:
	class ACharacter* OwnerCharacter;
};
