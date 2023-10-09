// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryTooltip.h"
#include "Items/ItemBase.h"
#include "Widgets/Inventory/InventoryItemSlot.h"


void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	UItemBase* ItemBeingHovered = InventorySlotBeingHobered->GetItemReferecne();



}