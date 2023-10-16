#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Datas/ItemDataStructs.h"
#include "CStatusComponent.generated.h"


class ACPlayer;

UENUM(BlueprintType)
enum class ECharacterSpeed : uint8
{
	Walk, Run, Sprint, Max,
};

USTRUCT()
struct FPlayerStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EStatTypes Type;

	UPROPERTY(EditAnywhere)
	float StatDefaultValue;

	UPROPERTY(EditAnywhere)
	float ModifierValue;

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




	// 배열로 스피드 관리 
	UPROPERTY(EditDefaultsOnly, Category = "Speed")
		float Speed[(int32)ECharacterSpeed::Max] = { 200, 400, 600 };


	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 100.f;



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

	void ApplyEquipmentStat();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	FPlayerStat HealthStat;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float Health = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float Stamina = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Mana")
	float Mana = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	float Armor = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "CritChance")
	float CritChance;

	UPROPERTY(EditDefaultsOnly, Category = "CritMultiplier")
	float CritMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "AttackSpeed")
	float AttackSpeed = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Block")
	float Block = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category = "MagicDamage")
	float MagicDamage = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "CastingSpeed")
	float CastingSpeed = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "bCanMove")
	bool bCanMove = true;


protected:

	UPROPERTY()
	ACPlayer* PlayerCharacter;
		
};
