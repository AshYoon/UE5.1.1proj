#include "Components/CStatusComponent.h"
#include "Global.h"
#include "Items/ItemBase.h"
#include "GameFramework/CharacterMovementComponent.h"



UCStatusComponent::UCStatusComponent()
{
	HealthStat = { EStatTypes::Health, 100.0f, 0.0f };


}



void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}


void UCStatusComponent::SetMove()
{
	bCanMove = true;
}

void UCStatusComponent::SetStop()
{
	bCanMove = false;
}


void UCStatusComponent::SetSpeed(ECharacterSpeed InType)
{
	// characterMovenment component ��� �ʿ� 
	UCharacterMovementComponent* movement = CHelpers::GetComponent<UCharacterMovementComponent>(GetOwner());

	movement->MaxWalkSpeed = Speed[(int32)InType];

}

void UCStatusComponent::AddHealth(float InAmount)
{
	Health += InAmount;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	//clamp - �θƽ� ���̹����� , �ƽ� �Ѿ�� �ƽ��θ� 

}

void UCStatusComponent::SubHealth(float InAmount)
{
	Health -= InAmount;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	//clamp - �θƽ� ���̹����� , �ƽ� �Ѿ�� �ƽ��θ� 
}

void UCStatusComponent::ApplyEquipmentStat()
{
	//TODO - Equipment���� Item�� valid���� Ȯ���ϰ� ItemStatics���� Modifier stat�� Modifier Value�� �޾Ƽ� 
	// �ش� StatType �� �����ش� 
	


}


