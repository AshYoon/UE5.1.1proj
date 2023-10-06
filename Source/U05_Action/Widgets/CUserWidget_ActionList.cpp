#include "Widgets/CUserWidget_ActionList.h"
#include "Global.h"
#include "CUserWidget_ActionItem.h"
#include "Components/GridPanel.h"
#include "Components/Border.h"

void UCUserWidget_ActionList::NativeConstruct()
{
	UGridPanel* gridPanel = Cast<UGridPanel>(GetWidgetFromName("Grid")); //GridPanel 접근해서 
	TArray<UWidget*> widgets = gridPanel->GetAllChildren(); // 자손을가질수없는애들은안되는데 , 

	for (UWidget* widget : widgets)
	{
		//for문돌려서 
		FUserWidget_Action_Data data;
		data.Item = Cast<UCUserWidget_ActionItem>(widget);

		Datas.Add(data); // 데이터 가져와서 Datas에 넣어주기 
	}

	Super::NativeConstruct();
}

void UCUserWidget_ActionList::Clicked(FString InName)
{
	// 참조로 가져와서 스코프로 돌려서 
	// Name이 같으면 델리게이트 바운딩해주고 
	for (const FUserWidget_Action_Data& data : Datas)
	{
		if (data.Item->GetName() == InName)
		{
			if (data.OnUserWidget_ActionItem_Clicked.IsBound())
				data.OnUserWidget_ActionItem_Clicked.Broadcast();

			SetVisibility(ESlateVisibility::Hidden);

			UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

			break;
		}
	}



}

void UCUserWidget_ActionList::Hovered(FString InName)
{
	for (const FUserWidget_Action_Data& data : Datas)
	{
		UBorder* border = Cast<UBorder>(data.Item->GetWidgetFromName("BG_Border"));

		if (data.Item->GetName() == InName)
		{
			border->SetBrushColor(FLinearColor::Red);

			break;
		}
	}
}

void UCUserWidget_ActionList::Unhovered(FString InName)
{
	for (const FUserWidget_Action_Data& data : Datas)
	{
		UBorder* border = Cast<UBorder>(data.Item->GetWidgetFromName("BG_Border"));

		if (data.Item->GetName() == InName)
		{
			border->SetBrushColor(FLinearColor::White);

			break;
		}
	}
}