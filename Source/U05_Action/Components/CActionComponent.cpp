#include "CActionComponent.h"
#include "Global.h"
#include "Actions/CAction.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"
#include "GameFramework/Character.h"

// ���� Action���� ���� Action�� ����Ѵ� 
//������ ActionData�� �޾ƛ����� �����ؾ��Ѵ�. 



UCActionComponent::UCActionComponent()
{

}


void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// �ش� �����Ϳ� ���� BeginPlay 

	ACharacter* character = Cast<ACharacter>(GetOwner());
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		// Data->DataAssets
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(character, &Datas[i]);
	}

}


void UCActionComponent::SetUnarmedMode()
{
	// �����Ǽ� ȣ���Ҽ������ϱ� Null Ȯ�� 

	// beginplay���� ������ Datas �� �����ϸ�ȴ� 

	if (!!Datas[(int32)Type])
	{
		// Data�� ���������־ 
		ACEquipment* equipment = Datas[(int32)Type]->GetEquipment();
		CheckNull(equipment);

		equipment->Unequip();
	}



	//ACEquipment* equipment = Datas[(int32)EActionType::Unarmed]->GetEquipment();
	//CheckNull(equipment);
	//// Unarmed ��� ���� Equipüũ���ʿ䰡���� 
	//equipment->Equip();






	ChangeType(EActionType::Unarmed);

}

void UCActionComponent::SetFistMode()
{

	SetMode(EActionType::Fist);

}

void UCActionComponent::SetOneHandMode()
{

	
	SetMode(EActionType::OneHand);
}

void UCActionComponent::SetTwoHandMode()
{

	SetMode(EActionType::TwoHand);

}
void UCActionComponent::SetWarpMode()
{

	SetMode(EActionType::Warp);

}
void UCActionComponent::SetFireStormMode()
{

	SetMode(EActionType::FireStorm);

}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	// ��������Ʈ �߰��ؼ� ����°Ŷ� �Ȱ��� 
	EActionType prevType = InNewType;
	Type = InNewType;

	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType, InNewType);

}

void UCActionComponent::SetMode(EActionType InType)
{
	if (Type == InType) //���� ���� ����
	{
		SetUnarmedMode();

		return;
	}
	else if (IsUnarmedMode() == false) //�ٸ� ���� ����
	{
		ACEquipment* equipment = Datas[(int32)Type]->GetEquipment();
		CheckNull(equipment);

		equipment->Unequip();
	}

	ACEquipment* equipment = Datas[(int32)InType]->GetEquipment();
	CheckNull(equipment);

	equipment->Equip();

	ChangeType(InType);


}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action)
			action->DoAction();
	}
}

void UCActionComponent::DoAim()
{
	SetAimMode(true); // ���Ӹ�� �ٲ��ֱ� 
}

void UCActionComponent::UndoAim()
{
	SetAimMode(false);
}

void UCActionComponent::SetAimMode(bool InAim)
{
	CheckTrue(IsUnarmedMode()); // ������ �ѻ��¿����� Aim�� �������� 

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action)
			InAim ? action->OnAim() : action->OffAim();
	}
}

void UCActionComponent::OffAllCollision()
{
	for (UCAction * data : Datas)
	{
		if (!!data == false)
			continue;

		if (!!data->GetAttachment() == false)
			continue;

		data->GetAttachment()->OffCollision();
	}
}

void UCActionComponent::SetIceBallMode()
{
	SetMode(EActionType::IceBall);
}

