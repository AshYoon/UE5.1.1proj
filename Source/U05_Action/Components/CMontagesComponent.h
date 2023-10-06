#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CStateComponent.h"
#include "Engine/DataTable.h"
#include "CMontagesComponent.generated.h"

// 몽타주를 받기위한 구조체 
USTRUCT(BlueprintType)
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EStateType Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U05_ACTION_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DataTable;


public:	

	UCMontagesComponent();

	// 플레이할 몽타주 
	void PlayRoll();
	void PlayBackstep();
	void PlayHitted();
	void PlayDead();
private:
	void PlayAnimMontage(EStateType InState);

private:

	// 생성할때 크기고 int32로 변환해서 그거에맞는 숫자를넣어주면 실행된다 ,다시한번확인 
	FMontageData* Datas[(int32)EStateType::Max];



protected:

	virtual void BeginPlay() override;


		
};
