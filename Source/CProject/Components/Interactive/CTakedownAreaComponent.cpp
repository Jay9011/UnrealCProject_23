#include "Components/Interactive/CTakedownAreaComponent.h"

#include "AddOn/Weapon/CAddOn_StealthKill.h"
#include "Character/CPlayer.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/BoxComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/AI/CAIBehaviorComponent.h"
#include "Components/Interactive/CInteractiveComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CWeaponObject.h"

UCTakedownAreaComponent::UCTakedownAreaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TakedownArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TakedownArea"));
	TakedownArea->SetupAttachment(this);
	TakedownArea->SetRelativeTransform(FTransform(FRotator(0.f, 0.f, 0.f), FVector(-60.f, 0.f, 0.f), FVector(1.f, 1.f, 1.f)));
	TakedownArea->SetBoxExtent(FVector(60.f, 50.f, 80.f));

	TakedownArea->CanCharacterStepUpOn = ECB_No;
	TakedownArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UCTakedownAreaComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerEnemy = Cast<ACEnemy_AI>(GetOwner());

	if (OwnerEnemy != nullptr)
	{
		OwnerBehaviorComponent = OwnerEnemy->GetBehaviorComponent();
	}

	TakedownArea->OnComponentBeginOverlap.AddDynamic(this, &UCTakedownAreaComponent::OnTakedownAreaBeginOverlap);
	TakedownArea->OnComponentEndOverlap.AddDynamic(this, &UCTakedownAreaComponent::OnTakedownAreaEndOverlap);
}

void UCTakedownAreaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCTakedownAreaComponent::OnTakedownAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* Player = Cast<ACPlayer>(OtherActor);
	CheckNull(Player);

	CheckTrue(Player == Cast<ACPlayer>(OwnerBehaviorComponent->GetTarget()));
	
	UCInteractiveComponent* InteractiveComponent = Cast<UCInteractiveComponent>(Player->GetComponentByClass(UCInteractiveComponent::StaticClass()));
	CheckNull(InteractiveComponent);
	
	if (CheckStealthKillable(Player))
	{
		InteractiveComponent->SetTarget(GetInteractTarget_Implementation(), this);		
	}
	else
	{
		InteractiveComponent->SetTarget(nullptr, this);
	}
}

void UCTakedownAreaComponent::OnTakedownAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPlayer* Player = Cast<ACPlayer>(OtherActor);
	CheckNull(Player);

	UCInteractiveComponent* InteractiveComponent = Cast<UCInteractiveComponent>(Player->GetComponentByClass(UCInteractiveComponent::StaticClass()));
	CheckNull(InteractiveComponent);

	InteractiveComponent->ClearTarget(this);
}

void UCTakedownAreaComponent::OnTakedownMontage(UAnimMontage* InMontage)
{
	if (InMontage != nullptr)
	{
		OwnerEnemy->GetStateComponent()->SetHittedMode();
		float Duration = OwnerEnemy->PlayAnimMontage(InMontage);
		
		FTimerManager& TimerManager = OwnerEnemy->GetWorldTimerManager();
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("OnTakedownEnd"));

		FTimerHandle TakedownTimerHandle;
		TimerManager.SetTimer(TakedownTimerHandle, TimerDelegate, Duration - 1.f, false);
	}
}

bool UCTakedownAreaComponent::CheckStealthKillable(ACPlayer* InPlayer)
{
	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(InPlayer->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullResult(WeaponComponent, false);

	UCWeaponObject* WeaponObject = Cast<UCWeaponObject>(WeaponComponent->GetWeapon());
	CheckNullResult(WeaponObject, false);
		
	bool bCan = WeaponObject->GetAddOnComponent<UCAddOn_StealthKill>() != nullptr;
	bCan &= IsEnemyInStealthKillableState();

	return bCan;
}

bool UCTakedownAreaComponent::IsEnemyInStealthKillableState()
{
	return	OwnerBehaviorComponent->IsWait()
		||	OwnerBehaviorComponent->IsPatrol()
		||	OwnerBehaviorComponent->IsVigilant();
}

void UCTakedownAreaComponent::OnTakedownEnd()
{
	UCStatusComponent* StatusComponent = OwnerEnemy->GetStatusComponent();
	CheckNull(StatusComponent);

	float MaxHealth = StatusComponent->GetMaxHealth();
	StatusComponent->Damage(MaxHealth);
}

void UCTakedownAreaComponent::OnInteract_Implementation(ACPlayer* InInteractPlayer)
{
	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(InInteractPlayer->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNull(WeaponComponent);

	UCWeaponObject* WeaponObject = Cast<UCWeaponObject>(WeaponComponent->GetWeapon());
	CheckNull(WeaponObject);

	UCAddOn_StealthKill* StealthKill = WeaponObject->GetAddOnComponent<UCAddOn_StealthKill>();
	CheckNull(StealthKill);

	StealthKill->OnStealthKill(OwnerEnemy, this);
}

bool UCTakedownAreaComponent::CanInteract_Implementation(ACPlayer* InInteractPlayer)
{
	return CheckStealthKillable(InInteractPlayer);
}

AActor* UCTakedownAreaComponent::GetInteractTarget_Implementation()
{
	return OwnerEnemy;
}

FText UCTakedownAreaComponent::GetInteractText_Implementation()
{
	return FText::FromString(TEXT("암살"));
}

