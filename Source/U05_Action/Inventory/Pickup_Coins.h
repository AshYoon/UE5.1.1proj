// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Pickup.h"
#include "Pickup_Coins.generated.h"

/**
 * 
 */
UCLASS()
class U05_ACTION_API APickup_Coins : public APickup
{
	GENERATED_BODY()
	

public:
	APickup_Coins();

	virtual void BeginPlay() override;

	virtual void Interact_Implementation() override;

public:

	UPROPERTY(EditAnywhere,Category = "Coin Properties")
	int32 AmountOfCoins;


};
