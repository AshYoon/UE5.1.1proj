// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryTooltip.h"
#include "Items/ItemBase.h"
#include "Widgets/Inventory/InventoryItemSlot.h"
#include "Components/TextBlock.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemBeingHovered = InventorySlotBeingHobered->GetItemReferecne();

	switch (ItemBeingHovered->ItemType)
	{
	case EItemType::Top:
		break;
	case EItemType::Arrows:
		break;
	case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		//SellValue->SetVisibility(ESlateVisibility::Collapsed);
		//ModifierType->SetVisibility(ESlateVisibility::Collapsed);
		//ModifierValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Feet:
		break;
	case EItemType::Hand:
		break;
	case EItemType::Head:
		ItemType->SetText(FText::FromString("Head"));
		break;
	case EItemType::Legs:
		break;
	case EItemType::Material:
		ItemType->SetText(FText::FromString("Material"));
		ModifierType->SetVisibility(ESlateVisibility::Collapsed);
		ModifierValue->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::MeleeWeapon:
		ItemType->SetText(FText::FromString("MeleeWeapon"));
		break;
	case EItemType::Necklace:
		break;
	case EItemType::RangeWeapon:
		ItemType->SetText(FText::FromString("RangeWeapon"));
		break;
	case EItemType::Quest:
		break;
	case EItemType::Ring:
		break;
	case EItemType::Shield:
		break;
	case EItemType::Tool:
		break;
	case EItemType::None:
		ItemType->SetText(FText::FromString("Mundane Item"));
		ModifierType->SetVisibility(ESlateVisibility::Collapsed);
		ModifierValue->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:
		break;
	}


	// Set ModifierType
	switch (ItemBeingHovered->ItemStatisics.Type)
	{
	case EStatTypes::Armor:
		ModifierType->SetText(FText::FromString("Armor"));
		break;
	case EStatTypes::Health:
		ModifierType->SetText(FText::FromString("Health"));
		break;
	case EStatTypes::Stamina:
		ModifierType->SetText(FText::FromString("Stamina"));
		break;
	case EStatTypes::Mana:
		ModifierType->SetText(FText::FromString("Mana"));
		break;
	case EStatTypes::Block:
		ModifierType->SetText(FText::FromString("Block"));
		break;
	case EStatTypes::CastingSpeed:
		ModifierType->SetText(FText::FromString("CastingSpeed"));
		break;
	case EStatTypes::AttackSpeed:
		ModifierType->SetText(FText::FromString("AttackSpeed"));
		break;
	case EStatTypes::CritChance:
		ModifierType->SetText(FText::FromString("CritChance"));
		break;
	case EStatTypes::CritMultiplier:
		ModifierType->SetText(FText::FromString("CritMultiplier"));
		break;
	case EStatTypes::Damage:
		ModifierType->SetText(FText::FromString("Damage"));
		break;
	case EStatTypes::MagicDamage:
		ModifierType->SetText(FText::FromString("MagicDamage"));
		break;
	case EStatTypes::MeleeAttackStaminaCost:
		ModifierType->SetText(FText::FromString("MeleeAttackStaminaCost"));
		break;
	case EStatTypes::None:
		//ModifierType->SetVisibility(ESlateVisibility::Collapsed);
		//ModifierValue->SetVisibility(ESlateVisibility::Collapsed);
		break;

	default:
		break;
	}



	ItemName->SetText(ItemBeingHovered->ItemTextData.Name);
	ModifierValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatisics.ModifierValue));
	UsageText->SetText(ItemBeingHovered->ItemTextData.UsageText);
	//SellValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatisics.SellValue));
	ItemDescription->SetText(ItemBeingHovered->ItemTextData.Description);
	StackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));

	if (ItemBeingHovered->NumbericData.bIsStackable)
	{
		MaxStackSize->SetText(FText::AsNumber(ItemBeingHovered->NumbericData.MaxStackSize));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
		MaxStackSizeText->SetVisibility(ESlateVisibility::Collapsed);
	}


}