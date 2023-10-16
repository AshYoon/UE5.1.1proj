// Fill out your copyright notice in the Description page of Project Settings.

/* Game*/
#include "Widgets/Equipment/EquipmentPanel.h"
#include "Widgets/Equipment/EquipmentSlot.h"
#include "Widgets/Inventory/InventoryItemSlot.h"
#include "Characters/CPlayer.h"
#include "Items/ItemBase.h"
#include "Widgets/Inventory/ItemDragDropOperation.h"
/* Engine */
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"

void UEquipmentPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<ACPlayer>(GetOwningPlayerPawn());

	if (PlayerCharacter)
	{
		EquipmentReference = PlayerCharacter->GetEquipment();
		if (EquipmentReference)
		{
			EquipmentReference->OnEquipmentUpdated.AddUObject(this, &UEquipmentPanel::RefreshEquipment);

		}
	}


}

bool UEquipmentPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{


	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (PlayerCharacter && ItemDragDrop->SourceItem)
	{
		PlayerCharacter->MoveItemToEquipment(ItemDragDrop->SourceItem);
		UE_LOG(LogTemp, Warning, TEXT("Detected and Item Drop on EquipmentPanel. "))
		return true;
	}
	return false;




}

void UEquipmentPanel::RefreshEquipment()
{
	


	UEquipmentSlot* EquipmentSlot = nullptr;

	if(!EquipmentSlotClass)
		UE_LOG(LogTemp, Warning, TEXT("Detected EquipmentReference or EquipmentSlotClass invalid. "))

	if (EquipmentReference && EquipmentSlotClass)
	{
		ClearWarpBox();
		for (UItemBase* const& EquipmentItem : EquipmentReference->GetEquipmentContents())
		{
			switch (EquipmentItem->GetItemType())
			{
			case EItemType::Head:
				EquipmentSlot = CreateWidget<UEquipmentSlot>(this, EquipmentSlotClass);
				EquipmentSlot->SetEquipmentReference(EquipmentItem);
				EquipmentHeadWarpBox->AddChildToWrapBox(EquipmentSlot);
				break;
			case EItemType::MeleeWeapon:
				EquipmentSlot = CreateWidget<UEquipmentSlot>(this, EquipmentSlotClass);
				EquipmentSlot->SetEquipmentReference(EquipmentItem);
				EquipmentWeaponWarpBox->AddChildToWrapBox(EquipmentSlot);
				break;
			case EItemType::Necklace:
				EquipmentSlot = CreateWidget<UEquipmentSlot>(this, EquipmentSlotClass);
				EquipmentSlot->SetEquipmentReference(EquipmentItem);
				EquipmentNecklace->AddChildToWrapBox(EquipmentSlot);
				break;
			case EItemType::Top:
				EquipmentSlot = CreateWidget<UEquipmentSlot>(this, EquipmentSlotClass);
				EquipmentSlot->SetEquipmentReference(EquipmentItem);
				EquipmentTopWarpBox->AddChildToWrapBox(EquipmentSlot);
				break;
			case EItemType::Legs:
				EquipmentSlot = CreateWidget<UEquipmentSlot>(this, EquipmentSlotClass);
				EquipmentSlot->SetEquipmentReference(EquipmentItem);
				EquipmentLegWarpBox->AddChildToWrapBox(EquipmentSlot);
				break;
			case EItemType::Feet:
				EquipmentSlot = CreateWidget<UEquipmentSlot>(this, EquipmentSlotClass);
				EquipmentSlot->SetEquipmentReference(EquipmentItem);
				EquipmentFeetWarpBox->AddChildToWrapBox(EquipmentSlot);
				break;
			default:
				break;
			}


		}

	}



}


void UEquipmentPanel::SetInfoText() const
{


}

void UEquipmentPanel::ClearWarpBox()
{
	EquipmentHeadWarpBox->ClearChildren();
	EquipmentWeaponWarpBox->ClearChildren();
	EquipmentTopWarpBox->ClearChildren();

	EquipmentLegWarpBox->ClearChildren();
	EquipmentFeetWarpBox->ClearChildren();

}