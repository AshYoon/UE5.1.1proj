#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CStateComponent.h"
#include "Engine/DataTable.h"
#include "CMontagesComponent.generated.h"

// ��Ÿ�ָ� �ޱ����� ����ü 
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

	// �÷����� ��Ÿ�� 
	void PlayRoll();
	void PlayBackstep();
	void PlayHitted();
	void PlayDead();
private:
	void PlayAnimMontage(EStateType InState);

private:

	// �����Ҷ� ũ��� int32�� ��ȯ�ؼ� �װſ��´� ���ڸ��־��ָ� ����ȴ� ,�ٽ��ѹ�Ȯ�� 
	FMontageData* Datas[(int32)EStateType::Max];



protected:

	virtual void BeginPlay() override;


		
};
