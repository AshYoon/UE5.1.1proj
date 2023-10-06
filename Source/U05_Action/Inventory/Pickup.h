// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/CInventoryComponent.h"
#include "Pickup.generated.h"


/**
 * 
 */

//Item Type



UCLASS()
class U05_ACTION_API APickup : public AInteractable
{
	GENERATED_BODY()

public:
		APickup();

		virtual void Interact_Implementation() override; 

		virtual void BeginPlay() override;



		UFUNCTION(BlueprintNativeEvent)
		void Use();
		virtual void Use_Implementation();

		

		enum EItemType : uint8
		{
			EquipmentItem,
			Coin,
			UsableItem,
			ETC,
		};






		UPROPERTY(EditAnywhere, Category = "Pickup Properties")
			UTexture2D* PickupThumbnail; // pickup 아이콘 or 썸네일 


		UPROPERTY(EditAnywhere, Category = "Pickup Properties")
			FString ItemName;


		


		UPROPERTY(EditAnywhere, Category = "Pickup Properties")
			int32 Value;



		// this is clled when the item is picked up and needs to be cleaned up from the level 
		void OnPickedUp();



		uint8 GetItemType() { return ItemType; };



		UPROPERTY(BlueprintReadWrite, VisibleAnywhere,Category = "Pickup Properties")
			class UBoxComponent* CollisionBox;


		UPROPERTY(BlueprintReadWrite, EditAnywhere)
			class UParticleSystemComponent* ItemParticle;


private:

		UPROPERTY(EditAnywhere, Category = "Pickup Properties")
			class UCInventoryComponent* PickUpInventroy;


protected:

	uint8 ItemType; // set ItemType

	
};
