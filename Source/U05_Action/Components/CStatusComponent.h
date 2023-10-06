#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterSpeed : uint8
{
	Walk, Run, Sprint, Max,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U05_ACTION_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCStatusComponent();

protected:

	virtual void BeginPlay() override;


private:


	//UPROPERTY(EditDefaultsOnly, Category = "Speed")
	//	float WalkSpeed = 200.0f;

	//UPROPERTY(EditDefaultsOnly, Category = "Speed")
	//	float RunkSpeed = 400.0f;

	//UPROPERTY(EditDefaultsOnly, Category = "Speed")
	//	float SprintSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 100;


	// 배열로 스피드 관리 
	UPROPERTY(EditDefaultsOnly, Category = "Speed")
		float Speed[(int32)ECharacterSpeed::Max] = { 200, 400, 600 };





public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }


	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ECharacterSpeed::Walk]; }
	FORCEINLINE float GetRunkSpeed() { return Speed[(int32)ECharacterSpeed::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ECharacterSpeed::Sprint]; }

	FORCEINLINE bool CanMove() {
		return bCanMove;
	}

	void SetMove();
	void SetStop();


	void SetSpeed(ECharacterSpeed InType);

	void AddHealth(float InAmount);
	void SubHealth(float InAmount);


private:

	float Health;

	bool bCanMove = true;
		
};
