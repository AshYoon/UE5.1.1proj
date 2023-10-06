#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	// uint8 -character형 

	Idle, Roll, Backstep, Equip, Action, Hitted, Dead, Max, Stun,

};
// Action은 모든 상황에서의 공격 
// Max는 우리가 0번부터 시작하니깐 Max 자체가 크기 이다 , 배열을만들거나하면 이 Max를 가져다 쓰면되고 
// 뭘 추가하려면 그 사이에 넣으면된다 , 즉 Max가 Enum문의 최대 갯수를 의미하게된다

// 이전타입 , 현재 타입을 받아서 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);







UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U05_ACTION_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()
		// 외부에서 접근할수있는 모드 타입 , 타입을 우리가 바꿔주는거다 
		// 모드를 바꾸고 싶으면 우리가 호출해서 바꿔줘야한다 
public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsRollMode() { return Type == EStateType::Roll; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsBackstepMode() { return Type == EStateType::Backstep; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }


	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
public:
	void SetIdleMode();
	void SetRollMode();
	void SetBackstepMode();
	void SetEquipMode();
	void SetActionMode();
	void SetHittedMode();
	void SetDeadMode();


	void SetStun();

	



public:
	UPROPERTY(BlueprintAssignable)
		FStateTypeChanged OnStateTypeChanged;
private:
	EStateType Type;
private:

	void ChangeType(EStateType InType);
	// 추가 적으로 뭘 만들때는 여기 안에서 추가하면된다 
	// 이게 정답은 아니지만 , 처음에 큰틀을 만들고 추가삭제가 용이하게 만들어야한다 
	// 그래서 각각의 기능을 나눈거다 
	// ChangeType을 외부에서 접근해서 델리게이트를 통해 변경해야한다 
	// 함수 원본접근안되고 지역함수니깐 객체에 소속되어있다 
	// 델리게이트를 쓰는 이유 - 언제든 수정될수있으니깐 소유하면 잘못사용될가능성이있으니깐
	// 델리게이트로 접근해서 기능만 사용함 


public:

	UCStateComponent();

protected:

	virtual void BeginPlay() override;



	
};