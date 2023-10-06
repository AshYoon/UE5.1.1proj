#include "Components/CStateComponent.h"
#include "Global.h"



UCStateComponent::UCStateComponent()
{


}



void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetRollMode()
{
	ChangeType(EStateType::Roll);
}

void UCStateComponent::SetBackstepMode()
{
	ChangeType(EStateType::Backstep);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::ChangeType(EStateType InType)
{
	// ������ ���õ� Ÿ��(type) �� �츮�� �����ϴ� InType���� �ٲ��ش� 
	EStateType type = Type;
	Type = InType;

	// ��������Ʈ�� ���� 
	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(type, InType);
}


void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetStun()
{
	ChangeType(EStateType::Stun);
}
