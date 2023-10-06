#include "Actions/CThrow.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACThrow::ACThrow()
{
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Sphere);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	InitialLifeSpan = 3.0f;

	Projectile->InitialSpeed = 4000.0f;
	Projectile->MaxSpeed = 8000.0f;
	Projectile->ProjectileGravityScale = 0.0f;


}


void ACThrow::BeginPlay()
{
	Super::	BeginPlay();

	TArray<USphereComponent*> components;
	GetComponents<USphereComponent>(components);

	for (USphereComponent* sphere : components)
		sphere->OnComponentBeginOverlap.AddDynamic(this, &ACThrow::OnComponentBeginOverlap);

	
}


void ACThrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!Explosion)
	{
		FTransform transform = ExplosionTransform;
		transform.AddToTranslation(SweepResult.Location);
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));
		// ImpactNormal이 아니라 ImpactPoint 로 바꿔줘도된다 , 차이는 없다 



		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, transform);
	}

	if (OnThrowBeginOverlap.IsBound())
		OnThrowBeginOverlap.Broadcast(SweepResult);
	// 델리게이트로 바운딩 해놓은거다 

	Destroy();
}



