#include "CHUD.h"
#include "Global.h"
#include "Widgets/MainMenu.h"
#include "Widgets/Interaction/InteractionWidget.h"
#include "Engine/Texture2D.h"
#include "Engine/Canvas.h"


ACHUD::ACHUD()
{
	CHelpers::GetAsset<UTexture2D>(&Texture, "Texture2D'/Game/Textures/T_Crosshair.T_Crosshair'");
	//Texture2D'/Game/Textures/T_Crosshair.T_Crosshair'

}


void ACHUD::BeginPlay()
{
	Super::BeginPlay();

	// format of create UI,Widget
	if (MainMenuClass)
	{
		/*UMainMenu::StaticClass()-> if we need this only for C++ use this but half of the widget is the graphical element
		and that only exists in the editor, so that is why we use UProperties */
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		/* similer with hidden */
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

	}

	if (InteractionWidgetClass)
	{

		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);

	}



}
void ACHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{


		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);


	}



}

void ACHUD::HideMenu()
{
	if (MainMenuWidget)
	{


		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);


	}

}

void ACHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);

	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}




}

void ACHUD::ShowInteractionWidget() const 
{
	if (InteractionWidget)
	{
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}

}

void ACHUD::HideInteractionWidget() const 
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
		CLog::Print("Hide", -1, 5.0f, FColor::Blue);
	}

}

void ACHUD::UpdateInteractionWidget(const FInteractableData * InteractionData) const 
{
	/*when character find interactable where we gonna to call this to pass this data in and then it will update */

	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}


		InteractionWidget->UpdateWidget(InteractionData);

	}




}



void ACHUD::DrawHUD()
{
	Super::DrawHUD();

	/*           CROSS HAIR                 */
	CheckFalse(bDraw);
	CheckNull(Texture);

	FVector2D center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f); // ���������� ���δ� 
	FVector2D size(Texture->GetSurfaceWidth() * 0.5f, Texture->GetSurfaceHeight() * 0.5f);
	FVector2D position = center - size; // ���߾� ���� ,���Ϳ��� size��ŭ ���� ? 

	FCanvasTileItem item(position, Texture->Resource, FLinearColor::White); // �ؽ�ó���̶� �츮�� ������ ���̶� ����
	item.BlendMode = SE_BLEND_Translucent;

	Canvas->DrawItem(item);
	/*                                      */






}
