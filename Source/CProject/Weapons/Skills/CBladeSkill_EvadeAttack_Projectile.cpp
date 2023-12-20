#include "CBladeSkill_EvadeAttack_Projectile.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Utilities/CheckMacros.h"

ACBladeSkill_EvadeAttack_Projectile::ACBladeSkill_EvadeAttack_Projectile()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Root);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(Sphere);
	Niagara->SetAutoActivate(false);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetAutoActivate(false);
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void ACBladeSkill_EvadeAttack_Projectile::BeginPlay()
{
	Super::BeginPlay();

	/* Collision 세팅 */
	if (Sphere != nullptr)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACBladeSkill_EvadeAttack_Projectile::OnSphereComponentBeginOverlap);
		Sphere->OnComponentEndOverlap.AddDynamic(this, &ACBladeSkill_EvadeAttack_Projectile::OnSphereComponentEndOverlap);
	}

	ActionData.InitActionData();
}

void ACBladeSkill_EvadeAttack_Projectile::ActiveSkill()
{
	Super::ActiveSkill();

	Niagara->Activate(true);
	ProjectileMovement->Activate(true);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
#if DEBUG_SKILL
	Sphere->SetHiddenInGame(false);
#endif

	SetLifeSpan(Lifetime);
}

void ACBladeSkill_EvadeAttack_Projectile::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (OtherCharacter != nullptr)
	{
		// 중복 피격 방지
		if (ActionData.DamagedCharacters.Contains(OtherCharacter))
			return;
		ActionData.DamagedCharacters.AddUnique(OtherCharacter);
		
		// 데미지 처리
		ActionData.ActionData[0].DamagedData.SendDamage(OwnerCharacter, this, OtherCharacter);
	}
}

void ACBladeSkill_EvadeAttack_Projectile::OnSphereComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
