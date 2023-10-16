// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Equipment/EquipmentSlot.h"
#include "Items/ItemBase.h"
#include "WIdgets/Inventory/ItemDragDropOperation.h"

/*Engine*/
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UEquipmentSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();


}

void UEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (EquipmentReference)
	{
		switch (EquipmentReference->ItemQuality)
		{
		case EItemQuality::Shoddy:
			EquipmentBorder->SetBrushColor(FLinearColor::Gray);
			break;
		case EItemQuality::Common:
			EquipmentBorder->SetBrushColor(FLinearColor::White);
			break;
		case EItemQuality::Quality:
			EquipmentBorder->SetBrushColor(FLinearColor::Blue);
			break;
		case EItemQuality::MasterWork:
			EquipmentBorder->SetBrushColor(FLinearColor::Red);
			break;
		case EItemQuality::GrandMaster:
			EquipmentBorder->SetBrushColor(FLinearColor(1.0f, 0.45f, 0.0f));
			break;

		default:
			break;
		}

		EquipmentIcon->SetBrushFromTexture(EquipmentReference->AssetData.Icon);


	}
}

FReply UEquipmentSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

void UEquipmentSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UEquipmentSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UEquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}
