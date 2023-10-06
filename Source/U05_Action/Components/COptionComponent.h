#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "COptionComponent.generated.h"

// class group ���� 
UCLASS( ClassGroup=(GameProject), meta=(BlueprintSpawnableComponent) )
class U05_ACTION_API UCOptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCOptionComponent();

protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
		float HorizontalLookRate = 45; // ī�޶�ȸ�� �����Ҳ��� �̰ɷ� 

	UPROPERTY(EditDefaultsOnly)
		float VerticalLookRate = 45;

public:
	FORCEINLINE float GetHorizontalLookRate() { return HorizontalLookRate; }
	FORCEINLINE float GetVerticalLookRate() { return VerticalLookRate; }

	
		
};
