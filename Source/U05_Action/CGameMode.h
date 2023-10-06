// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameMode.generated.h"

/**
 * 
 */
UCLASS()
class U05_ACTION_API ACGameMode : public AGameModeBase
{
	// 꼭 다시확인 뒤에 ACGameMode 
	// 
	GENERATED_BODY()
public:
		virtual void BeginPlay() override;


public:
	ACGameMode();

public:
	// 토글시에 IE_Press 의 값이 0 , release가 1 
	enum EHUDState : uint8
	{
		HS_Ingame,
		HS_Inventory,
		HS_Shop_General,
		HS_Shop_Weapon,
		HS_Equipment,
		HS_BattleMode,

	};

	//checks the hus state , and then calls apply "HUDs"  accordingly to  apply whatever hud we want
	void ApplyHUDChanges();
	// getter for hud state
	uint8 GetHUDState();

	// Setter for hud state, applies the new state and then calls applyHUDChanges
	UFUNCTION(BlueprintCallable , Category = "HUD Functions")
	void ChangeHUDState(uint8 NewState);


	// applies a hud to the screen , returns ture if successful , false otherwise
	// WidgetToApply -> Widget that we want to apply on screen
	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents);


protected:

	uint8 HUDState; // current HUDState must be protected 
	// the HUD to be shown ingame, protected blueprint variable
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> IngameHUDClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> InventoryHUDClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> ShopGeneralHUDClass;

	UPROPERTY()
		class UUserWidget* CurrentWidget;

};
