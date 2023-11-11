#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "CBaseCharacter.generated.h"

UCLASS()
class CPROJECT_API ACBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACBaseCharacter();

protected:
	virtual void BeginPlay() override;

/*
 * State 관련
 */
private:
	UFUNCTION()
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) {};

protected:
	UPROPERTY(VisibleAnywhere, Category = "Base")
	class UCStateComponent* State;
};
