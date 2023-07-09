#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBaseCharacter.generated.h"

UCLASS()
class CPROJECT_API ACBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere, Category = "State")
	class UCStateComponent* State;

public:
	ACBaseCharacter();

protected:
	virtual void BeginPlay() override;
};
