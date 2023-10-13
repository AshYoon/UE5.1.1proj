

#include "Components/CEquipmentComponent.h"
#include "Items/ItemBase.h"

UCEquipmentComponent::UCEquipmentComponent()
{




}


void UCEquipmentComponent::BeginPlay()
{


	Super::BeginPlay();


	
}


void UCEquipmentComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	/* ITem Switch �������� Equipment���ִ� ������ �����ϰ� Inventory�� �߰��ϱ� */

	EquipmentContents.RemoveSingle(ItemToRemove);

	/* call delegate , we gonna bind this delegate to ui */
	OnEquipmentUpdated.Broadcast();

}

FItemAddResult UCEquipmentComponent::HandleAddItem(UItemBase* InputItem)
{

	//TODO - if Equipment Contents�� ����Ÿ���� �ٸ� �������������ܿ� ��ü 
	// �ٸ��������� ������� ���� 
	if (GetOwner())
	{
		if (!InputItem->NumbericData.bIsStackable)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't added {0} to the Equipment.invalid item."),
				InputItem->ItemTextData.Name));
		}


	}

	return FItemAddResult();
}

UItemBase* UCEquipmentComponent::FindMathingItem(UItemBase* ItemIn) const
{
	// TODO ���� ITem Type �� ã��
	if (ItemIn)
	{
		if (EquipmentContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

FItemAddResult UCEquipmentComponent::FindNextItemByItemType(UItemBase* ItemIn)
{


	return FItemAddResult();
}









