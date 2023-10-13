

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
	/* ITem Switch 과정에서 Equipment에있는 아이템 제거하고 Inventory에 추가하기 */

	EquipmentContents.RemoveSingle(ItemToRemove);

	/* call delegate , we gonna bind this delegate to ui */
	OnEquipmentUpdated.Broadcast();

}

FItemAddResult UCEquipmentComponent::HandleAddItem(UItemBase* InputItem)
{

	//TODO - if Equipment Contents에 같은타입의 다른 아이템이있을겨우 교체 
	// 다른아이템이 없을경우 장착 
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
	// TODO 같은 ITem Type 을 찾기
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









