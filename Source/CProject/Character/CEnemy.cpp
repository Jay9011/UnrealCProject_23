#include "Character/CEnemy.h"

#include "Utilities/CLog.h"
#include "Weapons/CWeaponStructures.h"

ACEnemy::ACEnemy()
{
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damaged.Damage = Damage;
	Damaged.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damaged.Causer = DamageCauser;
	Damaged.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetHittedMode();
	
	return Damage;
}

void ACEnemy::Hitted()
{
	if (!Damaged.Event || !Damaged.Event->HitData)
		return;
	
	CLog::Log(Super::GetName() + " Hitted : " + FString::SanitizeFloat(Damaged.Damage));
	
	FHitData* HitData = Damaged.Event->HitData;

	HitData->PlayMontage(this);
	HitData->PlayHitStop(GetWorld());
	HitData->PlaySoundWave(this);

	// 중복 피격 방지
	Damaged.Damage = 0;
	Damaged.Character = nullptr;
	Damaged.Causer = nullptr;
	Damaged.Event = nullptr;
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted:
		Hitted();
		break;
	}
}
