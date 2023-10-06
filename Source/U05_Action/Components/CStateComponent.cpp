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
	// 이전에 세팅된 타입(type) 을 우리가 설정하는 InType으로 바꿔준다 
	EStateType type = Type;
	Type = InType;

	// 델리게이트로 연결 
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
