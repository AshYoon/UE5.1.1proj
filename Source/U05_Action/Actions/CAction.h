#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CAction.generated.h"

// CAction에서 사본을 관리해준다 


// 여기에 맞춰서 데이터를 넣는거다 
//USTRUCT로 구조체 생성 
USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

public:
	// 여기에 해당되는애들이 데이터 에셋 
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;
	// Mode에 따라서 다른 몽타주 실행하기위한 구조체 

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;

};


USTRUCT(BlueprintType)
struct FDoActionData : public FEquipmentData
{
	GENERATED_BODY()
		// DoAction에서 쓸 구조체 
public:
	UPROPERTY(EditAnywhere)
		float Power = 5.0f;

	UPROPERTY(EditAnywhere)
		float HitStop; // 경직 쓰려고 사용 

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;
	// 파티클 넣어서 공격에따라서 나오는 파티클 다르니깐 파티클이 생성될 오프셋 

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShakeBase> ShakeClass; // 카메라 쉐이크 

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACThrow> ThrowClass;

};




UCLASS()
class U05_ACTION_API UCAction : public UObject
{
	GENERATED_BODY()


public:
	friend class UCActionData; //friend선언으로 CActionData에 접근가능하게 

public:
	FORCEINLINE class ACEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class ACDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }

	FORCEINLINE FLinearColor GetEquipmentColor() { return EquipmentColor; }





private:
	class ACEquipment* Equipment;
	class ACAttachment* Attachment;
	class ACDoAction* DoAction;

	FLinearColor EquipmentColor;



	
};
