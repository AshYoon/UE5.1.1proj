#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "Actions/CAction.h" // CAction.헤더를 추가하면서 해결 
#include "GenericTeamAgentInterface.h"
#include "CEnemy.generated.h"

UCLASS()
class U05_ACTION_API ACEnemy : public ACharacter, public IICharacter , public IGenericTeamAgentInterface
{
	GENERATED_BODY()


		
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
		int32 ID = 0;


	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; } //team id 세팅 

private:
	UPROPERTY(EditAnywhere, Category = "Hitted")
		float LaunchAmount = 100.0f;
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActionComponent* Action;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

public:
	ACEnemy();


	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void BeginPlay() override;


	FGenericTeamId TeamID; //TeamID변수 

public:
	virtual void Tick(float DeltaTime) override;
	virtual void ChangeColor(FLinearColor InColor) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
	
	UFUNCTION()
		void RestoreColor();
private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;
private:
	void Hitted();// 피격처리할 함수 
	void Dead();
	// beginDead , EndDead도 필요하다 


public:
	virtual void Begin_Dead() override;
	virtual void End_Dead() override;
private:
	class AController* DamageInstigator; // 인스티게이터를 담을 변수 
	float DamageValue; // 데미지 변수 

};
