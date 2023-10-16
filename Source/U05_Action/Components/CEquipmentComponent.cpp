

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
	// PlayerCharacter Stat�� �޾Ƽ� Stat�� �ȸ´� Item �� �����������ϰ� �����ؾ��� 

		//if bIsStackable is false - > invalid item 

	if (!InputItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input item is somehownull"));
	}

	/* if maxstacksize is over 1 , it is not Equipable item */
	if (InputItem->NumbericData.MaxStackSize > 1)
	{
		//���� ����� �����°ź��� isStackable�� true�� �����Ǿ��ִ� 

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
		/*�̹� copy�Ⱦ������̰ų� ������ ��ġ�Ǿ��ִ� item�̶�� �ش� �������� �̹� �޸𸮿� �����Ǿ��ִ� ���� */
		/* if the item is already a copy , or is a world pickup */
		NewItem = Item;
		NewItem->ResetItemFlags();

	}
	else
	{
		// use when splitting or dragging to / from another inventory 
		/*���� �ٸ������ copy�ص��ȴ� */
		NewItem = Item->CreateItemCopy();
	}

	//NewItem->OwningInventory = this;
	NewItem->OwningEquipment = this;
	EquipmentContents.Add(NewItem);

	/* it could be stack */
	OnEquipmentUpdated.Broadcast();


}






