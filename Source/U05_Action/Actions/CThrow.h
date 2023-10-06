#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CThrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThrowBeginOverlap, FHitResult, InHitResult);


UCLASS()
class U05_ACTION_API ACThrow : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* Explosion;

	UPROPERTY(EditDefaultsOnly)
		FTransform ExplosionTransform;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;
private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(BlueprintAssignable)
		FThrowBeginOverlap OnThrowBeginOverlap;
public:	

	ACThrow();

protected:

	virtual void BeginPlay() override;



};
