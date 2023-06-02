#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "CPlayer.generated.h"

UCLASS()
class CPROJECT_API ACPlayer : public ACBaseCharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
	class UCameraComponent* Camera;
public:
	ACPlayer();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
