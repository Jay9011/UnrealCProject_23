#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utilities/CTraceUtil.h"
#include "DebugHeader.h"
#include "MyDebugger/IDebugCollector.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FVector LeftDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FVector RightDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FVector PelvisDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FRotator LeftRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FRotator RightRotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCFeetComponent : public UActorComponent
	, public IIDebugCollector
{
	GENERATED_BODY()

public:	
	UCFeetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void Trace(FName InSocketName, float& OutDistance, FRotator& OutRotation) const;
	
private:
	UPROPERTY(EditAnywhere, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float InterpSpeed = 50.f;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance = 50.f;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float OffsetDistance = 5.f;

	UPROPERTY(EditAnywhere, Category = "Trace")
	FName LeftSocketName = "Foot_L";

	UPROPERTY(EditAnywhere, Category = "Trace")
	FName RightSocketName = "Foot_R";

	UPROPERTY(EditAnywhere, Category = "Feet")
	float MaxRoll = 30.f;

	UPROPERTY(EditAnywhere, Category = "Feet")
	float MaxPitch = 15.f;
	
private:
	UPROPERTY()
	class ACharacter* OwnerCharacter;
	
	FFeetData FeetData;
	
/*
 * Getter
 */
public:
	FORCEINLINE FFeetData GetFeetData() const { return FeetData; }

#if DEBUG_FEET
	virtual bool IsDebugEnable() override;
	virtual FDebugInfo GetDebugInfo() override;
#endif
	
};