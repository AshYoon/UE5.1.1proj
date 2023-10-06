#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CAction.generated.h"

// CAction���� �纻�� �������ش� 


// ���⿡ ���缭 �����͸� �ִ°Ŵ� 
//USTRUCT�� ����ü ���� 
USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

public:
	// ���⿡ �ش�Ǵ¾ֵ��� ������ ���� 
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;
	// Mode�� ���� �ٸ� ��Ÿ�� �����ϱ����� ����ü 

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
		// DoAction���� �� ����ü 
public:
	UPROPERTY(EditAnywhere)
		float Power = 5.0f;

	UPROPERTY(EditAnywhere)
		float HitStop; // ���� ������ ��� 

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;
	// ��ƼŬ �־ ���ݿ����� ������ ��ƼŬ �ٸ��ϱ� ��ƼŬ�� ������ ������ 

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShakeBase> ShakeClass; // ī�޶� ����ũ 

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACThrow> ThrowClass;

};




UCLASS()
class U05_ACTION_API UCAction : public UObject
{
	GENERATED_BODY()


public:
	friend class UCActionData; //friend�������� CActionData�� ���ٰ����ϰ� 

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
