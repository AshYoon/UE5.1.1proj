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

	// ��Ÿ�� �������� Play �ϰ� �ƴҶ��� EndEquip�ϴϰ� ��� ���� ���������� 
	if (Data.AnimMontage != NULL)
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRatio, Data.StartSection);
	else
		End_Equip();

	// bPawnControl�� True�϶��� �ٲ�����Ѵ� , 



	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

	// Equip�� ����ɶ����� �� �ٲ�� 
	IICharacter* character = Cast<IICharacter>(OwnerCharacter);
	CheckNull(character);
	// �׸�忡 �´� ������ �ٲ𲨴� , 
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

