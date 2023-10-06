#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

//���� ����Ʈ�� �浹ó�� ���� 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttamentBeginOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttamentEndOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);




// �ݸ��� ��������Ʈ 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentCollision);
UCLASS()
class U05_ACTION_API ACAttachment : public AActor
{
	GENERATED_BODY()
protected:
	// ��Ʈ ������Ʈ��� ����ȴ� 
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
	// ��������Ʈ�� �Լ� 


public:

	//�浹ó�� 
	UPROPERTY(BlueprintAssignable)
		FAttamentBeginOverlap OnAttamentBeginOverlap;

	UPROPERTY(BlueprintAssignable)
		FAttamentEndOverlap OnAttamentEndOverlap;

	UPROPERTY(BlueprintAssignable)
		FAttachmentCollision OnAttachmentCollision;

	UPROPERTY(BlueprintAssignable)
		FAttachmentCollision OffAttachmentCollision;

private:
	// �浹ó�� ��缳�� 
	TArray<class UShapeComponent*> ShapeComponents;



};
