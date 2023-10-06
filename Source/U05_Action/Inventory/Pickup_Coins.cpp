// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Pickup_Coins.h"
#include "Characters/CPlayer.h"
#include "Global.h"

APickup_Coins::APickup_Coins()
{



	AmountOfCoins = 0;
	ItemType = EItemType::Coin;



}

void APickup_Coins::BeginPlay()
{
	Super::BeginPlay();
}



void APickup_Coins::Interact_Implementation()
{



	ACPlayer* Character = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	Character->UpdateGold(AmountOfCoins);

	Destroy();

}


