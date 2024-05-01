#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Interface/CDamagedInterface.h"
#include "CBaseCharacter.generated.h"

UCLASS()
class CPROJECT_API ACBaseCharacter : public ACharacter
	, public ICDamagedInterface
{
	GENERATED_BODY()

public:
	ACBaseCharacter();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void ApplyDamage() override;
	virtual void LaunchAttacker(const FRotator& InLookAtRotation) override;
	virtual void ResetDamagedData(FDamagedData& DamagedData) override;

public:
	FRotator LookAt(const ACharacter* InTarget);
	
private:
	UFUNCTION()
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) {};

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID = 0;

	FDamagedData Damaged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	class UCStatusComponent* Status;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere, Category = "Base")
	class UCMontagesComponent* Montages;

private:
	UPROPERTY(EditAnywhere, Category = "Mesh|Socket")
	FName HeadSocket = "head";
	UPROPERTY(EditAnywhere, Category = "Mesh|Socket")
	FName HeadForwardSocket = "Head_Forward";

/*
 * Getter
 */
public:
	UFUNCTION(BlueprintCallable, BlueprintGetter, Category = "Base")
	FORCEINLINE class UCStateComponent* GetStateComponent() const { return State; };
	FORCEINLINE class UCStatusComponent* GetStatusComponent() const { return Status; };

	FORCEINLINE uint8 GetTeamID() const { return TeamID; };

	FORCEINLINE FTransform GetHeadTransform(ERelativeTransformSpace TransformSpace = ERelativeTransformSpace::RTS_World) const;
	FORCEINLINE FVector GetHeadOffset(ERelativeTransformSpace TransformSpace = ERelativeTransformSpace::RTS_World) const;
	FORCEINLINE FTransform GetHeadForwardTransform(ERelativeTransformSpace TransformSpace = ERelativeTransformSpace::RTS_World) const;
};
