#include "CGameMode.h"
#include "Characters/CPlayer.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "CHUD.h"



ACGameMode::ACGameMode()
{


	// default pawn class는 기본적으로 있는애들이다 
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");


	//Blueprint'/Game/BP_CHUD.BP_CHUD'
	//C 기반 블프니깐 _C를 붙여주는거 잊지말자 
	//CHelpers::GetClass<AHUD>(&HUDClass, "Blueprint'/Game/BP_CHUD.BP_CHUD_C'");

	// use our custom hud 
	// use default hud of basic FPS template  in youtube 
	HUDClass = ACHUD::StaticClass();

	//Set Default HUDState to be ingame state
	HUDState = EHUDState::HS_Ingame;
	


}


void ACGameMode::BeginPlay()
{

	// as soon as the player enters the game , apply hud to screen
	ApplyHUDChanges();

}

void ACGameMode::ApplyHUDChanges()
{
	//Remove previous hud , since we 're applying a new one

	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();

	}

	switch (HUDState)
	{
	    case EHUDState::HS_Ingame:
	    {
		    ApplyHUD(IngameHUDClass, false, false);
		    break;
	    }
		case EHUDState::HS_Inventory:
		{
			ApplyHUD(InventoryHUDClass, true, true);
			break;
		}
		case EHUDState::HS_Shop_General:
		{
			ApplyHUD(ShopGeneralHUDClass, true, true);
			break;
		}
		default:
		{
			ApplyHUD(IngameHUDClass, false, false);
			break;

		}
	}






}

uint8 ACGameMode::GetHUDState()
{
	return HUDState;
}

void ACGameMode::ChangeHUDState(uint8 NewState)
{
	HUDState = NewState;
	ApplyHUDChanges();

}

bool ACGameMode::ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents)
{
	//get reference player , and controller
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	//null check whatever widget we 're trying to apply 
	if (WidgetToApply != nullptr)
	{
		//set mouse visibility and click events according to params
		MyController->bShowMouseCursor = bShowMouseCursor;
		MyController->bEnableClickEvents = EnableClickEvents;

		//create my widget
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);

		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
			return true;
		}
		else return false;
	}
	else return false;
}


