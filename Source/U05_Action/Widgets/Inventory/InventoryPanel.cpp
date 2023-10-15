// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryPanel.h"
#include "Characters/CPlayer.h"
#include "Widgets/Inventory/InventoryItemSlot.h"
#include "Widgets/Inventory/ItemDragDropOperation.h"
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
	//WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}")
	//	, InventoryReference->GetInventoryTotalWeight(),
	//	InventoryReference->GetWeightCapacity()));

	//CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
	//	InventoryReference->GetInventoryContents().Num(),
	//	InventoryReference->GetSlotsCapacity()));

	/* float 을 강제로 int 로 변환하는 부분 수정 */

	const FString WeightInfoValue{
		FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + "/"
		+ FString::SanitizeFloat(InventoryReference->GetWeightCapacity()) };

	const FString CapacityInfoValue{
		FString::FromInt(InventoryReference->GetInventoryContents().Num()) + "/"
		+ FString::FromInt(InventoryReference->GetSlotsCapacity()) };

	WeightInfo->SetText(FText::FromString(WeightInfoValue));
	CapacityInfo->SetText(FText::FromString(CapacityInfoValue));


}

void UInventoryPanel::RefreshInventory()
{

	if (InventoryReference && InvnetorySlotClass)
	{
		// clear the inventory ,
		//we want it to give the appearance of only showing the most up to date picture of inventory 
		// before we add everything
		InventoryWarpBox->ClearChildren();

		//iterate inventoryreference
		for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InvnetorySlotClass);
			//UInventoryItemSlot::StaticClass() ; only can use in c++ , we want graphical stuff

			ItemSlot->SetItemReference(InventoryItem);
			InventoryWarpBox->AddChildToWrapBox(ItemSlot);
		}


		SetInfoText();

	}


}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	/*if i have valid sourceItem And my inventorypanel is valid */
	if (ItemDragDrop->SourceItem && InventoryReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected and Item Drop on InventoryPanel. "))

			//returning true will stop the drop operation at this widget
		return true;
	}

	// returning false will cause the drop operation to fall through to underlying widget (if any)
	return false;
}
