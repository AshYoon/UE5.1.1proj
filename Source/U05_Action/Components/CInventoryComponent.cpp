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
			/* dereference pointer 역참조해서 리턴한다 만약 result와 InventoryContents ItemIn과 일치하면 */
			/*double pointer에서 normal pointer로 return */
			return *Result;
		}
	}
	return nullptr;
}

UItemBase * UCInventoryComponent::FindNextPartialStack(UItemBase * ItemIn) const
{
	/*find by predicate - checks with condition  */
	/* 최대 stack이 아닌 아이템을 람다 함수를 사용해서 찾기 */
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

	/* weight capa and total weight is float , 반올림 혹은 반내림 해서 소수점이 들어가지않게함 */
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
	만약 maxstack이 4인 아이템이 스택으로 3개씩 2개 존재하고 바닥에 같은 아이템으로 3스택이 놓여져있다면 
	해당 pickup을 집었을때 내 인벤토리안의 3개짜리 스택2개는 max stack이 되어 4 개짜리 2개가되고 바닥에는 1스택만남아있는 
	아이템이 놓여져야한다 */

	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		//invalid item data 
		return;
	}


	int32 AmountToDistribute = RequestedAddAmount;

	/*look for next partial stack that matches that item type 
	and check if the input item already exists in the inventory and is not a full stack */
	UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

	/*FindNextPartialStack에서 아이템을 찾지 못한다면 nullptr을 반환하기때문에 더이상 찾을 아이템이
	없을때까지 while문을 돌리게된다 */
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
	/*existing stack을 찾아서 추가를 했어도 stack에서 전부 소화하지못하고 남은 아이템이있을수있어서 
	이 if 문과 동시에 진행될수있다 */
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


				/*만약 AddNewItem을 할때 CreateItemCopy를 안한다면 ? 남아있는 분배할 Item Amount의 총 Weight이 
				내 WeightCapacity를 초과하는경우 일부는 우리가 Inventory에 AddNewItem으로 추가하지만 일부는 Ground에 남아있어야한다
				PickUp은 여전히 Ground에 일부 존재하고 이 메모리를 그대로 우리가 가져와서 추가하게된다면 Ground에 남아있는 Item과 
				내 인벤토리에 추가된 Item 둘다 같은 Inventory밖의 Ground의 메모리를 같은 시간에 참조하게되고 만약 Ground에 있는
				메모리가 사라진다면 Inventory에 추가된 Item의 메모리가 참조하는곳이 없어지므로 댕글링포인터가 발생하게된다 
				이를 방지하기위해 CreateItemCopy라는 함수를 제작 , 깊은 복사를 구현 */
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);

				return RequestedAddAmount - AmountToDistribute;
			}

			/*만약 If문이 False라면  ItemAmount를 전부다 추가할수있으므로 그냥 AddNewItem을 해줘도 된다  */
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

	NewItem->OwningInventory = this;
	NewItem->SetQuanity(AmountToAdd);

	InventoryContents.Add(NewItem);
	
	/* it could be stack */
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();

}







