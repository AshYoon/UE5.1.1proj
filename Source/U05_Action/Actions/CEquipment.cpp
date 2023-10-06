#include "Actions/CEquipment.h"
#include "Global.h"
#include "Characters/ICharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

ACEquipment::ACEquipment()
{

}


void ACEquipment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);


	Super::BeginPlay();
	
}

void ACEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	// 몽타주 있을때만 Play 하고 아닐때는 EndEquip하니간 없어도 딱히 문제가없다 
	if (Data.AnimMontage != NULL)
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRatio, Data.StartSection);
	else
		End_Equip();

	// bPawnControl이 True일때만 바꿔줘야한다 , 



	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

	// Equip이 실행될때마다 색 바뀌게 
	IICharacter* character = Cast<IICharacter>(OwnerCharacter);
	CheckNull(character);
	// 그모드에 맞는 색으로 바뀔꺼다 , 
	character->ChangeColor(Color);



}

void ACEquipment::Begin_Equip_Implementation()
{
	if (OnEquipmentDelegate.IsBound())
		OnEquipmentDelegate.Broadcast();
}

void ACEquipment::End_Equip_Implementation()
{
	bEquipped = true; 




	State->SetIdleMode();





}

void ACEquipment::Unequip_Implementation()
{
	
	bEquipped = false;


	if (OnUnequipmentDelegate.IsBound())
		OnUnequipmentDelegate.Broadcast();

	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;


}

