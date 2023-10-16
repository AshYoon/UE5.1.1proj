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
	// characterMovenment component 헤더 필요 
	UCharacterMovementComponent* movement = CHelpers::GetComponent<UCharacterMovementComponent>(GetOwner());

	movement->MaxWalkSpeed = Speed[(int32)InType];

}

void UCStatusComponent::AddHealth(float InAmount)
{
	Health += InAmount;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	//clamp - 민맥스 사이범위만 , 맥스 넘어가면 맥스로만 

}

void UCStatusComponent::SubHealth(float InAmount)
{
	Health -= InAmount;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	//clamp - 민맥스 사이범위만 , 맥스 넘어가면 맥스로만 
}

void UCStatusComponent::ApplyEquipmentStat()
{
	//TODO - Equipment에서 Item이 valid인지 확인하고 ItemStatics에서 Modifier stat과 Modifier Value를 받아서 
	// 해당 StatType 에 더해준다 
	


}


