#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Melee.generated.h"


UCLASS()
class U05_ACTION_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()
public:
	FORCEINLINE void EnableCombo() { bEnable = true; }

	FORCEINLINE void DisableCombo() { bEnable = false; }
	// bEnable이 있어야 다음으로 넘어갈지안넘어갈지 할수있으니깐 
public:
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

	virtual void OnAttachmentCollision();
	virtual void OffAttachmentCollision();

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;
private:
	UFUNCTION()
		void RestoreDilation();

private:
	// 콤보 존재여부 , index , 마지막콤보인지 
	bool bExist;
	bool bEnable;
	bool bLast;

	int32 Index;
	TArray<class ACharacter*> HittedCharacters;
	// 우리가 배열을 생성한는 방식 

};
