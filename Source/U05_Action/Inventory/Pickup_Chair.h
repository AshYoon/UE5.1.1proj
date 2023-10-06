// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Pickup.h"
#include "Pickup_Chair.generated.h"

/**
 * 
 */
UCLASS()
class U05_ACTION_API APickup_Chair : public APickup
{
	GENERATED_BODY()
	


public:
	APickup_Chair();

	// we want to chair do something
	virtual void Use_Implementation() override;
	


};
