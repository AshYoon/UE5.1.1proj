// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryItemSlot.h"
#include "Widgets/Inventory/InventoryTooltip.h"
#include "Items/ItemBase.h"
#include "Widgets/Inventory/DragItemVisual.h"
#include "WIdgets/Inventory/ItemDragDropOperation.h"
/*Game*/
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// ÅøÆÁ »ý¼º 
	if (ToolTipClass)
	{
		UInventoryTooltip* ToolTip = CreateWidget<UInventoryTooltip>(this, ToolTipClass);
		ToolTip->InventorySlotBeingHobered = this;
		
		SetToolTip(ToolTip);
	}



}
void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemReference)
	{
		switch (ItemReference->ItemQuality)
		{
		case EItemQuality::Shoddy:
			ItemBorder->SetBrushColor(FLinearColor::Gray);
			break;
		case EItemQuality::Common:
			ItemBorder->SetBrushColor(FLinearColor::White);
			break;
		case EItemQuality::Quality:
			ItemBorder->SetBrushColor(FLinearColor::Blue);
			break;
		case EItemQuality::MasterWork:
			ItemBorder->SetBrushColor(FLinearColor::Red);
			break;
		case EItemQuality::GrandMaster:
			ItemBorder->SetBrushColor(FLinearColor(1.0f,0.45f,0.0f));
			break;

		default:
			break;
		}

		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

		if (ItemReference->NumbericData.bIsStackable)
		{
			ItemQuanity->SetText(FText::AsNumber(ItemReference->Quanity));
		}
		else
		{
			ItemQuanity->SetVisibility(ESlateVisibility::Collapsed);

		}

	}



}





FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// take widget just return reference of the unerlying widget 
		/*if we hold or not move it , this won't fire */
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}


	// submenu on right click will happen here 

	return Reply.Unhandled();



}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	/* this only detected when mouse is dragging , 
	we need native on mouse button down in order to detect that we are left clicking  */

	if (DragItemVisualClass)
	{
		/*must use TObjectPtr in this , cause this widget is TObjectpointer and what we made is just DragItemVisual pointer*/
		const TObjectPtr<UDragItemVisual> DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

		/*check if item is stackable -> show itemquanity else collapsed*/
		ItemReference->NumbericData.bIsStackable
			? DragVisual->ItemQuanity->SetText(FText::AsNumber(ItemReference->Quanity))
			: DragVisual->ItemQuanity->SetVisibility(ESlateVisibility::Collapsed);



		/* this is data side , allow to carry data , just create in memory*/
		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;


		/*must use TObjectPtr in this , cause this widget is TObjectpointer and what we made is just DragItemVisual pointer*/
		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		/*  */
		OutOperation = DragItemOperation;

	}


}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}
