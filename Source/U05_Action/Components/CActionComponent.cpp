#include "CActionComponent.h"
#include "Global.h"
#include "Actions/CAction.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"
#include "GameFramework/Character.h"

// 이제 Action으로 만든 Action을 써야한다 
//원래는 ActionData를 받아썻지만 수정해야한다. 



UCActionComponent::UCActionComponent()
{

}


void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// 해당 데이터에 대한 BeginPlay 

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
	// 장착되서 호출할수있으니깐 Null 확인 

	// beginplay에서 만들어온 Datas 를 접근하면된다 

	if (!!Datas[(int32)Type])
	{
		// Data가 없을수도있어서 
		ACEquipment* equipment = Datas[(int32)Type]->GetEquipment();
		CheckNull(equipment);

		equipment->Unequip();
	}



	//ACEquipment* equipment = Datas[(int32)EActionType::Unarmed]->GetEquipment();
	//CheckNull(equipment);
	//// Unarmed 모드 갈때 Equip체크할필요가없다 
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
	// 델리게이트 추가해서 만드는거랑 똑같다 
	EActionType prevType = InNewType;
	Type = InNewType;

	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType, InNewType);

}

void UCActionComponent::SetMode(EActionType InType)
{
	if (Type == InType) //같은 무기 해제
	{
		SetUnarmedMode();

		return;
	}
	else if (IsUnarmedMode() == false) //다른 무기 장착
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
	SetAimMode(true); // 에임모드 바꿔주기 
}

void UCActionComponent::UndoAim()
{
	SetAimMode(false);
}

void UCActionComponent::SetAimMode(bool InAim)
{
	CheckTrue(IsUnarmedMode()); // 장착을 한상태에서만 Aim이 땡겨지게 

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

