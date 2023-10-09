// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryPanel.h"
#include "Characters/CPlayer.h"
#include "Widgets/Inventory/InventoryItemSlot.h"
#include "Widgets/Inventory/InventoryPanel.h"

//Engine
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<ACPlayer>(GetOwningPlayerPawn());

	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if (InventoryReference)
		{
			//DynamicDelegate->can use in BP but we use normal delegate
			/*anytime inventory changes we call refresh*/
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
			SetInfoText();



		}
	}
}

void UInventoryPanel::SetInfoText() const
{
	WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}")
		, InventoryReference->GetInventoryTotalWeight(),
		InventoryReference->GetWeightCapacity()));

	CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
		InventoryReference->GetInventoryContents().Num(),
		InventoryReference->GetSlotsCapacity()));




}

void UInventoryPanel::RefreshInventory()
{

	if (InventoryReference && InvnetorySlotClass)
	{
		// clear the inventory ,
		//we want it to give the appearance of only showing the most up to date picture of inventory 
		// before we add everything
		InventoryPanel->ClearChildren();

		//iterate inventoryreference
		for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InvnetorySlotClass);
			//UInventoryItemSlot::StaticClass() ; only can use in c++ , we want graphical stuff

			ItemSlot->SetItemReference(InventoryItem);
			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}
	}


}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);


	//return false;
}
