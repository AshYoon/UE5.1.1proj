// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Components/CInventoryComponent.h"

// meeber initializer list , initialize variable at the time of constructor , initialize befor the body of constructor
UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{


}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;

}


UItemBase * UItemBase::CreateItemCopy() const 
{
	// const - > to notice this variable must not change , don't need to change variable -> use const to know 
	// Inventory will make with pointers , try to merge with other items
	// Create whole new object , 



	//if Grab this item , is Template to make a new item 
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quanity = this->Quanity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->ItemTextData = this->ItemTextData;
	ItemCopy->NumbericData = this->NumbericData;
	ItemCopy->ItemStatisics = this->ItemStatisics;
	ItemCopy->AssetData = this->AssetData;


	ItemCopy->bIsCopy = true;

	return ItemCopy;
}



void UItemBase::SetQuanity(const int32 NewQuanity)
{
	if (NewQuanity != Quanity)
	{
		// if bIsStackable is true , stack size will be numericdata maxstacksize , or false size stackable size is 1
		//
		Quanity = FMath::Clamp(NewQuanity, 0, this->NumbericData.bIsStackable ? NumbericData.MaxStackSize : 1);


		if (this->OwningInventory)
		{
			if (this->Quanity <= 0)
			{
				//amount <= 0 , remove item , it's mean use last item or loose item 
				this->OwningInventory->RemoveSingleInstanceOfItem(this);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ItemBase Owning Inventory was null(item may be a pickup) "));
		}
	}



}

void UItemBase::Use(ACPlayer * Character)
{




}
