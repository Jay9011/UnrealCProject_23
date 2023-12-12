#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "UDirectionalUtilities.generated.h"

USTRUCT(BlueprintType)
struct FDirectionalValue
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float Forward = 0;
	UPROPERTY(EditAnywhere)
	float Right = 0;
};
/**
 * @brief 방향과 회전에 대한 유틸리티
 */
UCLASS()
class CPROJECT_API UDirectionalUtil: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// 현재 컨트롤러에 입력된 축 값으로 월드 좌표 기준 방향을 구한다.
	UFUNCTION(BlueprintCallable, Category = "Directional")
	static FVector GetWorldDirectionFromInputAxis(const ACharacter* InCurrentCharacter, const FName& InForwardAxis, const FName& InRightAxis)
	{
		const AController* Controller = InCurrentCharacter->GetController();
		if(!Controller)
			return FVector::ZeroVector;
		
		// 컨트롤러의 시점을 기준으로 회전 값을 가져온다.
		FRotator Rotation = Controller->GetControlRotation();

		// Z축 회전값을 0으로 만들어서 XY 평면으로 만든다.
		Rotation.Pitch = 0.0f;
		FVector ForwardDirection = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

		FVector Direction = ForwardDirection * InCurrentCharacter->GetInputAxisValue(InForwardAxis) + RightDirection * InCurrentCharacter->GetInputAxisValue(InRightAxis);
		Direction.Z = 0;
		
		return Direction;
	}
	// 입력 방향과 현재 컨트롤 중인 액터와의 방향성 일치 정도를 구한다.
	UFUNCTION(BlueprintPure, Category = "Directional")
	static FDirectionalValue GetDirectionalValue(const FVector& InDirection, const AActor* InCurrentActor)
	{
		FVector InputDir = InDirection.GetSafeNormal();
		
		// 캐릭터의 각 방향 벡터를 구한다.
		FVector Forward = InCurrentActor->GetActorForwardVector();
		FVector Right = InCurrentActor->GetActorRightVector();
		Forward.Z = 0;
		Right.Z = 0;

		// 내적을 통해 더 가까운 방향을 구한다.
		float ForwardDot = FVector::DotProduct(Forward, InputDir);
		float RightDot = FVector::DotProduct(Right, InputDir);

		return {ForwardDot, RightDot};
	}
	
	// 현재 컨트롤중인 액터와 입력 방향에 대한 회전 값을 구한다.
	static FRotator GetRotationFromInputDirection(const FVector& InDirection, const AActor* InCurrentActor)
	{
		return GetRotationFromInputDirection(InDirection, InCurrentActor->GetActorRotation());
	}
	// 사용자 입력 방향과 회전 값에 대한 회전값을 구한다.
	static FRotator GetRotationFromInputDirection(const FVector& InDirection, const FRotator& InCurrentRotation)
	{
		FQuat TargetRot = InDirection.ToOrientationQuat();
		FQuat CurrentRot = FQuat(InCurrentRotation);

		return FRotator(TargetRot * CurrentRot);
	}
};
/*
 * @breif 위치와 관련된 유틸리티
 */
UCLASS()
class UCPositionUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 현재 캐릭터의 카메라 컴포넌트를 가지고 화면의 중심부터 InLength 만큼의 거리에 해당하는 Start와 End를 구한다.
	UFUNCTION(BlueprintPure, Category = "Position")
	static void GetScreenCenterLocationWithACharacter(const ACharacter* InOwnerCharacter, float InLength, float InRadius, FVector& OutStart, FVector& OutEnd)
	{
		UCameraComponent* CameraComponent = Cast<UCameraComponent>(InOwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
		if (CameraComponent == nullptr)
			return;

		USpringArmComponent* SpringArmComponent = Cast<USpringArmComponent>(CameraComponent->GetAttachParent());
		// 스프링 암을 사용하는 경우
		if (SpringArmComponent != nullptr)
		{
			// 카메라 위치를 기준으로 스프링암 길이만큼 떨어진 위치를 구한다. 
			OutStart = CameraComponent->GetComponentLocation() + (CameraComponent->GetForwardVector() * SpringArmComponent->TargetArmLength);
		}
		// 스프링 암을 사용하지 않는 경우
		else
		{
			// 스프링암이 없다면, 카메라 위치가 시작 위치가 된다.
			OutStart = CameraComponent->GetComponentLocation();
		}
		
		// 카메라 위치를 기준으로 InLength 만큼 떨어진 위치를 구한다.
		OutEnd = CameraComponent->GetComponentLocation() + (CameraComponent->GetForwardVector() * InLength);
	}
};
