#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TimelineComponent.h"
#include "CAim.generated.h"


UCLASS()
class U05_ACTION_API UCAim : public UObject
{
	GENERATED_BODY()

		// aim하면 플레이어는 aim을가지는데 적은 가질필요없다
		// 여기서 구분지을필요가있고 던지는 동작도있어야하고 
public:
	FORCEINLINE bool IsAvaliable() { return SpringArm != NULL && Camera != NULL; }
	FORCEINLINE bool IsZoom() { return bInZoom; }

public:
	UCAim();
	void BeginPlay(class ACharacter* InCharacter);

	void OnZoom();
	void OffZoom();

	void Tick(float DeltaTime);

private:
	UFUNCTION()
		void Zooming(float Output);

private:
	class UCurveFloat* Curve;

	class ACHUD* HUD;

	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

	bool bInZoom;

	FTimeline Timeline;
	FOnTimelineFloat TimelineFloat;


	
};
