// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CPickup.h"
#include "Items/ItemBase.h"
#include "Components/CInventoryComponent.h"
#include "Global.h"

ACPickup::ACPickup()
{
 	PrimaryActorTick.bCanEverTick = false; // we don't need tick 

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");

	PickupMesh->SetSimulatePhysics(false); // set physics , it could be performance issue

	SetRootComponent(PickupMesh);
}


void ACPickup::BeginPlay()
{
	Super::BeginPlay();
	// Set id , Quanity
	InitializePickup(UItemBase::StaticClass(), ItemQuanity);

}





void ACPickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuanity)
{
	/*get the itemdata from table and spawn new one */
	CLog::Print("initialize pickup called ", -1, 3.f, FColor::Red);
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		CLog::Print("initialized success", -1, 3.f, FColor::Red);
		/*rowname = FName , contextstring = my item id */
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this, BaseClass);
		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->NumbericData = ItemData->NumbericData;
		ItemReference->ItemTextData = ItemData->ItemTextData;
		ItemReference->AssetData = ItemData->AssetData;

		InQuanity <= 0 ? ItemReference->SetQuanity(1) : ItemReference->SetQuanity(InQuanity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);


		UpdataInteractableData();




	}

}

void ACPickup::InitializeDrop(UItemBase * ItemToDrop, const int32 InQuanity)
{
	/*when you drop something and it's already in the inventory , it should be initialized from what's in the inventory
	rather than going searching datatable*/


	ItemReference = ItemToDrop;

	// safe check 
	InQuanity <= 0 ? ItemReference->SetQuanity(1) : ItemReference->SetQuanity(InQuanity);

	ItemReference->NumbericData.Weight = ItemToDrop->GetItemSingleWeight();
	
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);


	UpdataInteractableData();

}


void ACPickup::UpdataInteractableData()
{
	/* any time something change with the pickup , just call this */

	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->ItemTextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->ItemTextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quanity;
	
	InteractableData = InstanceInteractableData;





}






void ACPickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}




void ACPickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}


void ACPickup::Interact(ACPlayer * PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}
}



void ACPickup::TakePickup(const ACPlayer * Taker)
{


	/*is this item in the process of being deleted ? = ispendingkillpending */
	/* 만약 플레이어가 줍기 키를 연타했을경우를 방지하는 역할 , */
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{

			// try to add item to player inventory
			// based on result of the add operation
			// adjust or destroy the pickup 
			if (UCInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);
				/*EItemAddResult 에 따라 case 작성   */


				switch(AddResult.OperationResult)
				{
				case EItemAddResult::IAR_NoItemAdded:
					break;
				case EItemAddResult::IAR_PartialAmountItemAdded:

					UpdataInteractableData(); // data update for pickup 
					Taker->UpdateInteractionWidget(); // update ui for player 
					break;
				case EItemAddResult::IAR_AllItemAdded:


					Destroy();
					break;
				}

				/*역참조(Dereference) 해야지만 print 가능 , */
				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player inventory component is null!"));
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("pickup internal item reference was somehow null"));
		}


	}


}

#if WITH_EDITOR
void ACPickup::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{

	Super::PostEditChangeProperty(PropertyChangedEvent);

	// if change property valid 
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// if checking 
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(ACPickup, DesiredItemID))
	{
		if (ItemDataTable)
		{

			/*if we get valid row */
			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}


}

#endif