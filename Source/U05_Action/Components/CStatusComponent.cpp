#include "Components/CStatusComponent.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"



UCStatusComponent::UCStatusComponent()
{



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


