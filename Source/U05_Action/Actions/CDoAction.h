#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionData.h"
#include "CDoAction.generated.h"

UCLASS()
class U05_ACTION_API ACDoAction : public AActor
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetDatas(TArray<FDoActionData> InDatas) { Datas = InDatas; }
	// ActionData에서 DoActionData접근 
	FORCEINLINE void SetEquipped(const bool* InEquipped) { bEquipped = InEquipped; }

public:	

	ACDoAction();
public:
	virtual void DoAction() {}
	virtual void Begin_DoAction() {}
	virtual void End_DoAction() {}

	virtual void OnAim() {}
	virtual void OffAim() {}

public:
	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {}

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {}

	UFUNCTION()
		virtual void OnAttachmentCollision() {}

	UFUNCTION()
		virtual void OffAttachmentCollision() {}

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

protected:

	TArray<FDoActionData> Datas;
protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	const bool* bEquipped;
	

};
