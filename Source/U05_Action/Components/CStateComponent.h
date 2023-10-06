#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	// uint8 -character�� 

	Idle, Roll, Backstep, Equip, Action, Hitted, Dead, Max, Stun,

};
// Action�� ��� ��Ȳ������ ���� 
// Max�� �츮�� 0������ �����ϴϱ� Max ��ü�� ũ�� �̴� , �迭������ų��ϸ� �� Max�� ������ ����ǰ� 
// �� �߰��Ϸ��� �� ���̿� ������ȴ� , �� Max�� Enum���� �ִ� ������ �ǹ��ϰԵȴ�

// ����Ÿ�� , ���� Ÿ���� �޾Ƽ� 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);







UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U05_ACTION_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()
		// �ܺο��� �����Ҽ��ִ� ��� Ÿ�� , Ÿ���� �츮�� �ٲ��ִ°Ŵ� 
		// ��带 �ٲٰ� ������ �츮�� ȣ���ؼ� �ٲ�����Ѵ� 
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
	// �߰� ������ �� ���鶧�� ���� �ȿ��� �߰��ϸ�ȴ� 
	// �̰� ������ �ƴ����� , ó���� ūƲ�� ����� �߰������� �����ϰ� �������Ѵ� 
	// �׷��� ������ ����� �����Ŵ� 
	// ChangeType�� �ܺο��� �����ؼ� ��������Ʈ�� ���� �����ؾ��Ѵ� 
	// �Լ� �������پȵǰ� �����Լ��ϱ� ��ü�� �ҼӵǾ��ִ� 
	// ��������Ʈ�� ���� ���� - ������ �����ɼ������ϱ� �����ϸ� �߸����ɰ��ɼ��������ϱ�
	// ��������Ʈ�� �����ؼ� ��ɸ� ����� 


public:

	UCStateComponent();

protected:

	virtual void BeginPlay() override;



	
};