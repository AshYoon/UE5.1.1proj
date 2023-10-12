// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"

// Item Quality 
UENUM(BlueprintType)
enum class EItemQuality : uint8
{
	Shoddy UMETA(DisplayName = "Shoddy"), 
	Common UMETA(DisplayName = "Common"),
	Quality UMETA(DisplayName = "Quality"),
	MasterWork UMETA(DisplayName = "MasterWork"),
	GrandMaster UMETA(DisplayName = "GrandMaster"),

};


// Item Type 
UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Shield UMETA(DisplayName = "Shield"),
	Head UMETA(DisplayName = "Head"),
	Top UMETA(DisplayName = "Top"),
	Legs UMETA(DisplayName = "Legs"),
	Hand UMETA(DisplayName = "Hand"),
	Feet UMETA(DisplayName = "Feet"),
	Arrows UMETA(DisplayName = "Arrows"),
	Tool UMETA(DisplayName = "Tool"),
	Quest UMETA(DisplayName = "Quest"),
	Consumable UMETA(DisplayName = "Consumable"),
	Material UMETA(DisplayName = "Material"),
	Ring UMETA(DisplayName = "Ring"),
	MeleeWeapon UMETA(DisplayName = "MeleeWeapon"),
	RangeWeapon UMETA(DisplayName = "RangeWeapon"),
	Necklace UMETA(DisplayName = "Necklace"),
};



UENUM(BlueprintType)
enum class EStatTypes : uint8
{
	None UMETA(DisplayName = "None"),
	Health UMETA(DisplayName = "Health"),
	Stamina UMETA(DisplayName = "Stamina"),
	Mana UMETA(DisplayName = "Mana"),
	Damage UMETA(DisplayName = "Damage"),
	Armor UMETA(DisplayName = "Armor"),
	CritChance UMETA(DisplayName = "CritChance"),
	CritMultiplier UMETA(DisplayName = "CritMultiplier"),
	AttackSpeed UMETA(DisplayName = "AttackSpeed"),
	Block UMETA(DisplayName = "Block"),
	MeleeAttackStaminaCost UMETA(DisplayName = "MeleeAttackStaminaCost"),
	MagicDamage UMETA(DisplayName = "MagicDamage"),
	CastingSpeed UMETA(DisplayName = "CastingSpeed"),


};


// Check Item Type to increase stat and value 
USTRUCT()
struct FItemStatisics
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EStatTypes Type; 
	// check StatType 


	UPROPERTY(EditAnywhere)
	float ModifierValue; // increase player stat

	UPROPERTY(EditAnywhere)
	float SellValue; // increase player stat
};


// Name- ItemName , Description - Potion - can heal hp , InteractionText - 50 is healed , Usage Text - 50 or 100 ~~
USTRUCT()
struct FItemTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	UPROPERTY(EditAnywhere)
	FText UsageText; // drink or use or sell , drop 
};



// is this stackable or what is maxStacksize , or check weight 
USTRUCT()
struct FItemNumericData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	//doesn't need to edit this cause we have maxStacksize
	UPROPERTY()
	bool bIsStackable;

};


USTRUCT()
struct FItemAssetData
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};


//Table Row Base Sturct
USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()



	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere , Category = "Item Data")
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatisics ItemStatisics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData ItemTextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData NumbericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;

	
};




/**
 * 
 */
UCLASS()
class U05_ACTION_API UItemDataStructs : public UDataTable
{
	GENERATED_BODY()
	
};
