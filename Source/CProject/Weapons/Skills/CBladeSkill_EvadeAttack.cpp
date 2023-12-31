#include "CBladeSkill_EvadeAttack.h"

#include "NiagaraComponent.h"
#include "Character/CPlayer.h"
#include "Components/SphereComponent.h"
#include "Effect/PlayerEffectComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"

ACBladeSkill_EvadeAttack::ACBladeSkill_EvadeAttack()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(Root);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(Root);
	NiagaraComponent->SetAutoActivate(false);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(Root);
	ParticleSystemComponent->SetAutoActivate(false);
}

void ACBladeSkill_EvadeAttack::BeginPlay()
{
	Super::BeginPlay();

	/* Owner와 관련된 초기화 부분 */
	if(OwnerCharacter)
	{
		OwnerPlayer = Cast<ACPlayer>(OwnerCharacter);

		if(OwnerPlayer)
		{
			PlayerEffectComponent = Cast<UPlayerEffectComponent>(OwnerCharacter->GetComponentByClass(UPlayerEffectComponent::StaticClass()));
			
			// 이펙트 실행
			if (PlayerEffectComponent != nullptr)
			{
				PlayerEffectComponent->ArmLengthEffect(OwnerPlayer->GetSpringArm(), TargetArmLength);
				TArray<AActor*> IgnoreActors;
				PlayerEffectComponent->DilationEffect(OwnerCharacter, TargetDilation, IgnoreActors);
			}

		}
	}

	/* Niagara 인터페이스 세팅 */
	if (NiagaraComponent != nullptr)
	{
		NiagaraComponent->SetNiagaraVariableObject("Mesh_Around", this);
		NiagaraComponent->OnSystemFinished.AddDynamic(this, &ACBladeSkill_EvadeAttack::OnNiagaraSystemFinished);
	}

	/* Collision 세팅 */
	if (SphereComponent != nullptr)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACBladeSkill_EvadeAttack::OnSphereComponentBeginOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ACBladeSkill_EvadeAttack::OnSphereComponentEndOverlap);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ActionData.InitActionData();
}

void ACBladeSkill_EvadeAttack::ActiveSkill()
{
	Super::ActiveSkill();

	NiagaraComponent->Activate(true);
	ParticleSystemComponent->Activate(true);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
#if DEBUG_SKILL
	SphereComponent->SetHiddenInGame(false);
#endif

	// Addon Skill 실행
	{
		if (AddOnSkillClass != nullptr)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = OwnerCharacter;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FTransform SpawnTransform = OwnerCharacter->GetActorTransform();

			ACSkill* Skill = OwnerCharacter->GetWorld()->SpawnActor<ACSkill>(AddOnSkillClass, SpawnTransform, SpawnParams);
			
			if (AddOnSkillDelay > 0)
			{
				FTimerDelegate AddOnSkillDelayDelegate;
				AddOnSkillDelayDelegate.BindLambda([Skill]()
				{
					Skill->ActiveSkill();
				});
				OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(AddOnSkillDelayTimerHandle, AddOnSkillDelayDelegate, AddOnSkillDelay, false);
			}
			else
			{
				Skill->ActiveSkill();
			}
		}
	}
	
	// SphereComponent 종료 설정
	{
		FTimerDelegate SphereNocollisionDelegate;
		SphereNocollisionDelegate.BindLambda([this]()
		{
			SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
#if DEBUG_SKILL
			SphereComponent->SetHiddenInGame(true);
#endif
		});
		FTimerHandle NoCollisionTimerHandle;
		OwnerPlayer->GetWorld()->GetTimerManager().SetTimer(NoCollisionTimerHandle, SphereNocollisionDelegate, SphereLifetime, false);
	}
}

void ACBladeSkill_EvadeAttack::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
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
		ActionData.ActionData[0].DamagedData.SendDamage(OwnerCharacter, this, OtherCharacter, OverlappedComp);
	}
}

void ACBladeSkill_EvadeAttack::OnSphereComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ACBladeSkill_EvadeAttack::OnNiagaraSystemFinished(UNiagaraComponent* PSystem)
{
	Destroy();
}
