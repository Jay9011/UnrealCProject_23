#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSkill.generated.h"

class USceneComponent;
class ACharacter;

UCLASS(Abstract, NotBlueprintable)
class CPROJECT_API ACSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSkill();

protected:
	virtual void BeginPlay() override;

public:
	virtual void ActiveSkill() {}

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

protected:
	UPROPERTY()
	ACharacter* OwnerCharacter;
};
