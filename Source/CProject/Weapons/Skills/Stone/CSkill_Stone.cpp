#include "Weapons/Skills/Stone/CSkill_Stone.h"

#include "Character/CPlayer.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"

ACSkill_Stone::ACSkill_Stone()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Mesh 설정
	{
		StoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoneMesh"));
		StoneMesh->SetupAttachment(RootComponent);

		UStaticMesh* Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/CastleRiver/Meshes/SM_Stone09.SM_Stone09'")).Object;
		if (Mesh)
		{
			StoneMesh->SetStaticMesh(Mesh);		
		}
		StoneMesh->SetSimulatePhysics(false);
		StoneMesh->BodyInstance.bNotifyRigidBodyCollision = false;
		StoneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// ProjectileMovement 설정
	{
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
		ProjectileMovement->InitialSpeed = 1000.f;
		ProjectileMovement->bShouldBounce = true;
		ProjectileMovement->SetAutoActivate(false);
	}
}

void ACSkill_Stone::BeginPlay()
{
	Super::BeginPlay();
}

void ACSkill_Stone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACSkill_Stone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bPhysicsOn == false && OnSimulatePhysics())
	{
		bPhysicsOn = true;
	}
}

void ACSkill_Stone::ActiveSkill()
{
	FVector Direction = GetActorForwardVector();
	ActiveSkill(Direction);
}

void ACSkill_Stone::ActiveSkill(FVector& InDirection)
{
	ProjectileMovement->Velocity = InDirection * ProjectileMovement->InitialSpeed;
	ProjectileMovement->Activate();

	StoneMesh->BodyInstance.bNotifyRigidBodyCollision = true;
	StoneMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StoneMesh->OnComponentHit.AddDynamic(this, &ACSkill_Stone::OnHit);
	
	SetLifeSpan(LifeSpan);
}

bool ACSkill_Stone::OnSimulatePhysics()
{
	TArray<FHitResult> HitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetActorLocation(), GetActorLocation(), GetSphereRadius(), ObjectTypes, false, {}, EDrawDebugTrace::None, HitResults, true);

	for (const FHitResult& HitResult : HitResults)
	{
		if (Cast<ACPlayer>(HitResult.Actor.Get()))
		{
			return false;
		}
	}
	
	StoneMesh->SetSimulatePhysics(true);
	
	return true;
}

void ACSkill_Stone::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bPlayedHitSound)
	{
		const FVector Location = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Location);
		FAINoiseEvent NoiseEvent(OwnerCharacter, Location, 1.f, 1000.f);
		bPlayedHitSound = true;
	}
}

float ACSkill_Stone::GetProjectileSpeed() const
{
	return ProjectileMovement->InitialSpeed;
}

float ACSkill_Stone::GetSphereRadius() const
{
	return 30.f;
}
