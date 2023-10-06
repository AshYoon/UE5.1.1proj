#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Health.generated.h"

UCLASS()
class U05_ACTION_API UCUserWidget_Health : public UUserWidget
{
	GENERATED_BODY()
public:



public:
	UFUNCTION(BlueprintImplementableEvent)
		void Update(float Health, float MaxHealth);

};
