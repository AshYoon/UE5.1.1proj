#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CActionComponent.h"
#include "Components/CFeetComponent.h"
#include "CAnimInstance.generated.h"
// TypeChange가 ActionComponent에 있으니깐 이걸 가지고와야한다 

UCLASS()
class U05_ACTION_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EActionType ActionType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FFeetData FeetData;


public:
	// ABP를 위해 필요한 override 함수 
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;



	
private:
	UFUNCTION()
		void OnActionTypeChanged(EActionType InPrevType, EActionType InNewType);
};
