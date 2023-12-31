#include "Weapons/CAttachment.h"

#include "CGameMode.h"
#include "DrawDebugHelpers.h"
#include "GameModeInfoCustomizer.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShapeComponent.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"

ACAttachment::ACAttachment()
{
	Root = this->CreateDefaultSubobject<USceneComponent>("Root");
	this->SetRootComponent(Root);
}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	// Root가 가지고 있는 모든 자식 컴포넌트들을 가져온다.
	TArray<USceneComponent*> ComponentChildren;
	Root->GetChildrenComponents(true, ComponentChildren);
	for (USceneComponent* Child : ComponentChildren)
	{
		UShapeComponent* Shape = Cast<UShapeComponent>(Child);
		if (Shape == nullptr)
			continue;

		Collisions.Add(Shape);

		// 충돌체의 이벤트를 바인딩한다.
		Shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
		Shape->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
		
	}// for (UShapeComponent* Child : Children)

	// 첫 시작시 부착물의 모든 충돌체를 비활성화한다.
	OffCollisions();

	// 각종 설정이 포함된 게임 모드를 가져온다.
	GameMode = Cast<ACGameMode>(OwnerCharacter->GetWorld()->GetAuthGameMode());
	
	Super::BeginPlay();
}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ACAttachment::OnCollisions()
{
	if (OnAttachmentBeginCollision.IsBound())
		OnAttachmentBeginCollision.Broadcast();
	
	for (UShapeComponent* Collision : Collisions)
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

#if DEBUG_ATTACHMENT
		// 부착물의 충돌체 체널이 GameMode의 EvadableObjectType과 같다면
		if(GameMode != nullptr && CollisionChannelCheck(Collision))
		{
			Collision->SetHiddenInGame(false);
		}
#endif
	}
}

void ACAttachment::OffCollisions()
{
	if (OnAttachmentEndCollision.IsBound())
		OnAttachmentEndCollision.Broadcast();
	
	for (UShapeComponent* Collision : Collisions)
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
#if DEBUG_ATTACHMENT
		Collision->SetHiddenInGame(true);
#endif
	}
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == nullptr);
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor), OverlappedComponent, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == nullptr);
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<ACharacter>(OtherActor), OverlappedComponent, OtherComp, OtherBodyIndex);
}

bool ACAttachment::CollisionChannelCheck(const UShapeComponent* InShapeComponent)
{
	if (GameMode == nullptr || InShapeComponent == nullptr)
		return false;

	ECollisionResponse Response = InShapeComponent->GetCollisionResponseToChannel(UEngineTypes::ConvertToCollisionChannel(GameMode->GetEvadableType()));

	return Response != ECollisionResponse::ECR_Ignore;
}
