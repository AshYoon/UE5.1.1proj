

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
	// PlayerCharacter Stat을 받아서 Stat에 안맞는 Item 은 장착하지못하게 변경해야함 

		//if bIsStackable is false - > invalid item 

	if (!InputItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input item is somehownull"));
	}

	/* if maxstacksize is over 1 , it is not Equipable item */
	if (InputItem->NumbericData.MaxStackSize > 1)
	{
		//지금 여기로 들어오는거보니 isStackable이 true로 설정되어있다 

		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Couldn't added {0} to the Equipment.invalid item."),
			InputItem->ItemTextData.Name));
	}

	// if Equipmentcontents already have this item 
	if (FindMathingItem(InputItem))
	{

		UE_LOG(LogTemp, Warning, TEXT("Input item is already Equipped"));

		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Couldn't added {0} to the Equipment.already Equipped."),
			InputItem->ItemTextData.Name));
	}

	if (SameTypeValid(InputItem))
	{
		SwitchItem(InputItem, SameTypeValid(InputItem));


		return FItemAddResult::AddedPartial(1, FText::Format(FText::FromString(
			"Successfully added a {0} to the Equipment and chagne with {1} . "), 
			InputItem->ItemTextData.Name , SameTypeValid(InputItem)->ItemTextData.Name));
	}



	AddNewItem(InputItem);

	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString(
		"Successfully added a {0} to the Equipment . "), InputItem->ItemTextData.Name));
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

UItemBase* UCEquipmentComponent::SameTypeValid(UItemBase* ItemIn)
{
	if (ItemIn)
	{
		for (UItemBase* const& EquipmentItem : EquipmentContents)
		{
			// is EquipmentContents have sameType Item of ItemIn , it will return Item
			if (EquipmentItem->GetItemType() == ItemIn->GetItemType())
				return EquipmentItem;
		}
	}

	return nullptr;
}

bool UCEquipmentComponent::IsTypeValid(EItemType ItemType)
{
	for (UItemBase* const& EquipmentItem : EquipmentContents)
	{
		if (EquipmentItem->GetItemType() == ItemType)
		{
			return true;
		}
	}



	return false;
}

void UCEquipmentComponent::SwitchItem(UItemBase* ItemInput, UItemBase* ItemToRemove)
{


}

void UCEquipmentComponent::ApplyStat()
{


}

void UCEquipmentComponent::AddNewItem(UItemBase* Item)
{
	/* all the items are pointer , */
	UE_LOG(LogTemp, Warning, TEXT("Add New Item Called"));
	UItemBase* NewItem;

	//some checks 
	if (Item->bIsCopy || Item->bIsPickup)
	{
		/*이미 copy된아이템이거나 레벨에 배치되어있던 item이라면 해당 아이템은 이미 메모리에 참조되어있는 상태 */
		/* if the item is already a copy , or is a world pickup */
		NewItem = Item;
		NewItem->ResetItemFlags();

	}
	else
	{
		// use when splitting or dragging to / from another inventory 
		/*만약 다른경우라면 copy해도된다 */
		NewItem = Item->CreateItemCopy();
	}

	//NewItem->OwningInventory = this;
	NewItem->OwningEquipment = this;
	EquipmentContents.Add(NewItem);

	/* it could be stack */
	OnEquipmentUpdated.Broadcast();


}






