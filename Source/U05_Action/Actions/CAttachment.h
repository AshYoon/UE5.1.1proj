#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

//델리 게이트로 충돌처리 연결 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttamentBeginOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttamentEndOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);




// 콜리전 델리게이트 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentCollision);
UCLASS()
class U05_ACTION_API ACAttachment : public AActor
{
	GENERATED_BODY()
protected:
	// 루트 컴포넌트라고 보면된다 
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USceneComponent* Scene;



	UFUNCTION(BlueprintCallable)
		void AttachToCollision(class UShapeComponent* InComponent, FName InSocketName);
public:	

	ACAttachment();

protected:

	virtual void BeginPlay() override;
protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;


protected:
	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketName);


public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUnequip();

	void OnCollision();
	void OffCollision();


private:

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// 델리게이트용 함수 


public:

	//충돌처리 
	UPROPERTY(BlueprintAssignable)
		FAttamentBeginOverlap OnAttamentBeginOverlap;

	UPROPERTY(BlueprintAssignable)
		FAttamentEndOverlap OnAttamentEndOverlap;

	UPROPERTY(BlueprintAssignable)
		FAttachmentCollision OnAttachmentCollision;

	UPROPERTY(BlueprintAssignable)
		FAttachmentCollision OffAttachmentCollision;

private:
	// 충돌처리 모양설정 
	TArray<class UShapeComponent*> ShapeComponents;



};
