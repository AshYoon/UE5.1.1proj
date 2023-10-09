// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryItemSlot.h"
#include "Widgets/Inventory/InventoryTooltip.h"
#include "Items/ItemBase.h"

/*Game*/
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
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
			ItemBorder->SetBrushColor(FLinearColor(100.f,65.f,0.0f,1.f));
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
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}
