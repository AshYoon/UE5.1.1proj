#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_ActionItem.generated.h"


UCLASS()
class U05_ACTION_API UCUserWidget_ActionItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
		void Click();

	UFUNCTION(BlueprintCallable)
		void Hover();

	UFUNCTION(BlueprintCallable)
		void Unhover();

private:
	class UCUserWidget_ActionList* GetActionList();





};
