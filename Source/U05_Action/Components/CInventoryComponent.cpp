#include "Components/CInventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/ItemBase.h"
#include "Global.h"






UCInventoryComponent::UCInventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}



void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();





}

UItemBase * UCInventoryComponent::FindMathingItem(UItemBase * ItemIn) const
{
	if (ItemIn)
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}



UItemBase * UCInventoryComponent::FindNextItemById(UItemBase * ItemIn) const
{
	if (ItemIn)
	{
		/*Get ItemBase pointer to result and findbykey with ItemIn*/
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			/* dereference pointer �������ؼ� �����Ѵ� ���� result�� InventoryContents ItemIn�� ��ġ�ϸ� */
			/*double pointer���� normal pointer�� return */
			return *Result;
		}
	}
	return nullptr;
}

UItemBase * UCInventoryComponent::FindNextPartialStack(UItemBase * ItemIn) const
{
	/*find by predicate - checks with condition  */
	/* �ִ� stack�� �ƴ� �������� ���� �Լ��� ����ؼ� ã�� */
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
			InventoryContents.FindByPredicate([&ItemIn](const UItemBase* InventoryItem)
		{
					/*this is not pointer compare , just item ID compare to check is this same item */
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
			
		}
		))
	{
		return *Result;
	}



	return nullptr;


}

int32 UCInventoryComponent::CaculateWeightAddAmoint(UItemBase *ItemIn, int32 RequestedAddAmount)
{

	/* weight capa and total weight is float , �ݿø� Ȥ�� �ݳ��� �ؼ� �Ҽ����� �����ʰ��� */
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());

	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}

	return WeightMaxAddAmount;
}

int32 UCInventoryComponent::CaculateNumberForFullStack(UItemBase * StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumbericData.MaxStackSize - StackableItem->Quanity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);

}
void UCInventoryComponent::RemoveSingleInstanceOfItem(UItemBase * ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);

	/* call delegate , we gonna bind this delegate to ui */
	OnInventoryUpdated.Broadcast();

}

int32 UCInventoryComponent::RemoveAmountOfItem(UItemBase * ItemIn, int32 DesiredAmountToRemove)
{
	// safety check always lesser
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quanity);

	ItemIn->SetQuanity(ItemIn->Quanity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();


	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;

}
void UCInventoryComponent::SplitExistingStack(UItemBase * ItemIn, const int32 AmointToSplit)
{
	/*if adding one more thing would overflow your inventory , this will be false */
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmointToSplit);
		AddNewItem(ItemIn, AmointToSplit);


	}



}

FItemAddResult UCInventoryComponent::HandleNonStackableItem(UItemBase * InputItem  )
{

	/*using GetStackWeight to nonstackableItem reason is just generic check , it could be use inputitem get single weight */

	// check if in the input item has valid weight , this should never be true 
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemStackWeight() < 0)
	{
		//return added none 
		CLog::Print("Item has notvalid weight ", -1, 10.f, FColor::Red);

		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			"Could not add {0} to the inventory . Item has invalid weight value . "),InputItem->ItemTextData.Name));
	}

	
	/*will the item weight overflow weight capacity*/
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			"Could not add {0} to the inventory . Item would overflow weight limit . "), InputItem->ItemTextData.Name));
	}

	// adding one more item would overflow slot capacity 
	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			"Could not add {0} to the inventory . All inventory slots are full . "), InputItem->ItemTextData.Name));
	}

	AddNewItem(InputItem, 1);
	//return added all request
	return FItemAddResult::AddedAll(1,FText::Format(FText::FromString(
		"Successfully added a single {0} to the inventory . "),  InputItem->ItemTextData.Name));

}


int32 UCInventoryComponent::HandleStackableItems(UItemBase *ItemIn, int32 RequestedAddAmount)
{
	/* if i have max stack of 4 ,and we have 2 existing stack of 3 each 
	in ground three stack on , 
	3 -> 4 , 4 and ground will be 1 
	���� maxstack�� 4�� �������� �������� 3���� 2�� �����ϰ� �ٴڿ� ���� ���������� 3������ �������ִٸ� 
	�ش� pickup�� �������� �� �κ��丮���� 3��¥�� ����2���� max stack�� �Ǿ� 4 ��¥�� 2�����ǰ� �ٴڿ��� 1���ø������ִ� 
	�������� ���������Ѵ� */

	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		//invalid item data 
		return;
	}


	int32 AmountToDistribute = RequestedAddAmount;

	/*look for next partial stack that matches that item type 
	and check if the input item already exists in the inventory and is not a full stack */
	UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

	/*FindNextPartialStack���� �������� ã�� ���Ѵٸ� nullptr�� ��ȯ�ϱ⶧���� ���̻� ã�� ��������
	���������� while���� �����Եȴ� */
	while (ExistingItemStack)
	{
		/*if we find partial stack , if max stack is 4 and i have 2 , this will return 2 */
		const int32 AmountToMakeFullStack = CaculateNumberForFullStack(ExistingItemStack, AmountToDistribute);

		/*caculate how many of the AmountToMakeFullStack can actually be carried based on weight capacity   */
		const int32 WeightLimitAddAmount = CaculateWeightAddAmoint(ExistingItemStack, AmountToMakeFullStack);


		//as long as the remaining amoint of the item does not overflow weight capacity 
		if (WeightLimitAddAmount > 0)
		{


			ExistingItemStack->SetQuanity(ExistingItemStack->Quanity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);


			AmountToDistribute -= WeightLimitAddAmount;


			ItemIn->SetQuanity(AmountToDistribute);


			// TODO: refine this logic since going over weight capacity should not ever be possible
			//if max capacity is reached , no need to run the loop again
			if (InventoryTotalWeight >= InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				//return what we added
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if (WeightLimitAddAmount <= 0)
		{
			//if AmountToDistribute still left some of it 
			if (AmountToDistribute != RequestedAddAmount)
			{
				/*this block will be reachec if distributing an item across multiple stacks
				* and the weight limit is hit during that process 
				*/

				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}

			
			return 0;
		}


		if (AmountToDistribute <= 0)
		{
			// all of the input item was distributed across existing stacks 
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		// check if there is still another valid partial stack of the input item 
		ExistingItemStack = FindNextPartialStack(ItemIn);

	}


	// no more partial stack found , check if a new stack can be added
	/*existing stack�� ã�Ƽ� �߰��� �߾ stack���� ���� ��ȭ�������ϰ� ���� ���������������־ 
	�� if ���� ���ÿ� ����ɼ��ִ� */
	if (InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		// attemp to add as many from the remaining item quanity that can fit inventory weight capacity 
		const int32 WeightLimitAddAmount = CaculateWeightAddAmoint(ItemIn, AmountToDistribute);

		if (WeightLimitAddAmount > 0)
		{
			//if there is still more item to distribute ,but weight limit has been reached 
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				// adjust the input item and add a new stack with as many as can be held 
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuanity(AmountToDistribute);


				/*���� AddNewItem�� �Ҷ� CreateItemCopy�� ���Ѵٸ� ? �����ִ� �й��� Item Amount�� �� Weight�� 
				�� WeightCapacity�� �ʰ��ϴ°�� �Ϻδ� �츮�� Inventory�� AddNewItem���� �߰������� �Ϻδ� Ground�� �����־���Ѵ�
				PickUp�� ������ Ground�� �Ϻ� �����ϰ� �� �޸𸮸� �״�� �츮�� �����ͼ� �߰��ϰԵȴٸ� Ground�� �����ִ� Item�� 
				�� �κ��丮�� �߰��� Item �Ѵ� ���� Inventory���� Ground�� �޸𸮸� ���� �ð��� �����ϰԵǰ� ���� Ground�� �ִ�
				�޸𸮰� ������ٸ� Inventory�� �߰��� Item�� �޸𸮰� �����ϴ°��� �������Ƿ� ��۸������Ͱ� �߻��ϰԵȴ� 
				�̸� �����ϱ����� CreateItemCopy��� �Լ��� ���� , ���� ���縦 ���� */
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);

				return RequestedAddAmount - AmountToDistribute;
			}

			/*���� If���� False���  ItemAmount�� ���δ� �߰��Ҽ������Ƿ� �׳� AddNewItem�� ���൵ �ȴ�  */
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;

		}
	}


	OnInventoryUpdated.Broadcast();
	return RequestedAddAmount - AmountToDistribute;
}

FItemAddResult UCInventoryComponent::HandleAddItem(UItemBase * InputItem)
{
	if (GetOwner())
	{
			const int32 InitialRequestedAddAmount = InputItem->Quanity;

			/*for nonStackable item */
			if (InputItem->NumbericData.bIsStackable)
			{
					return HandleNonStackableItem(InputItem );
			}

			// handle stackable 
			const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

			if (StackableAmountAdded == InitialRequestedAddAmount)
			{
					// return add all result 
				return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(
					FText::FromString("Successfully added {0} {1} to the inventory. "),
					InitialRequestedAddAmount,
					InputItem->ItemTextData.Name));
			}

			if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
			{
					// return added partial result 
				return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
					FText::FromString("Partial amount of {0} added to the inventory.Number added = {1} "),
					InputItem->ItemTextData.Name,
					StackableAmountAdded));
			}

			if (StackableAmountAdded <= 0)
			{
					//return added non result 
				return FItemAddResult::AddedNone(FText::Format(
					FText::FromString("Couldn't added {0} to the inventory.No remaining inventory slots , or invalid item."),
					InputItem->ItemTextData.Name));
			}
	


	}

	return FItemAddResult();
}


void UCInventoryComponent::AddNewItem(UItemBase * Item, const int32 AmountToAdd)
{
	/* all the items are pointer , */

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

	NewItem->OwningInventory = this;
	NewItem->SetQuanity(AmountToAdd);

	InventoryContents.Add(NewItem);
	
	/* it could be stack */
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();

}







