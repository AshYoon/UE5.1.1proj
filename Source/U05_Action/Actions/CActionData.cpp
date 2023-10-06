#include "CActionData.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "CAction.h"

// �̷������� �����ֱ� 
void UCActionData::BeginPlay(ACharacter* InOwnerCharacter, UCAction** OutAction)
{
	FTransform transform;


	ACAttachment* attachment = NULL; // null ������ �ʱ�ȭ 



	// ��� ���� ������ü�� ������ �������� �����Ѵٰ� ����ȴ�  
	//Attachment
	if(!!AttachmentClass)
	{
		attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);
		attachment->SetActorLabel(GetLableName(InOwnerCharacter, "Attachment"));
		UGameplayStatics::FinishSpawningActor(attachment, transform);
	}


	ACEquipment* equipment = NULL;



	//Equipment
	if(!!EquipmentClass)
	{
		equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		equipment->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		equipment->SetActorLabel(GetLableName(InOwnerCharacter, "Equipment"));
		equipment->SetData(EquipmentData);
		equipment->SetColor(EquipmentColor);
		UGameplayStatics::FinishSpawningActor(equipment, transform);
		if (!!AttachmentClass)
		{
			equipment->OnEquipmentDelegate.AddDynamic(attachment, &ACAttachment::OnEquip);
			equipment->OnUnequipmentDelegate.AddDynamic(attachment, &ACAttachment::OnUnequip);
		}
	}


	ACDoAction* doAction = NULL;

	//DoAction
	if(!!DoActionClass)
	{
	   doAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionClass, transform, InOwnerCharacter);
	   doAction->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	   doAction->SetActorLabel(GetLableName(InOwnerCharacter, "DoAction"));
	   doAction->SetDatas(DoActionDatas);
	   UGameplayStatics::FinishSpawningActor(doAction, transform);

	   if (!!equipment)
	   {
		   doAction->SetEquipped(equipment->GetEquipped());
	   }



	   if (!!attachment)
	   {
		   // Attachment�� overlap�� collision�� DoActionClass�� ���� 
		   attachment->OnAttamentBeginOverlap.AddDynamic(doAction, &ACDoAction::OnAttachmentBeginOverlap);
		   attachment->OnAttamentEndOverlap.AddDynamic(doAction, &ACDoAction::OnAttachmentEndOverlap);

		   attachment->OnAttachmentCollision.AddDynamic(doAction, &ACDoAction::OnAttachmentCollision);
		   attachment->OffAttachmentCollision.AddDynamic(doAction, &ACDoAction::OffAttachmentCollision);
	   }


	}

	// 

	// �����Ҷ� new object�� �ϴ°��� action���� 
	//�����ٰ� �������� ����ȴ� , ���� ������ �츮�� �ȿ��� ���� ���ϰ� , �̰� ������� DA ���� ���ϴϱ�
	// �ָ� ȣ���ϴ°� Action Component 

	*OutAction = NewObject<UCAction>();
	(*OutAction)->Attachment = attachment;
	(*OutAction)->Equipment = equipment;
	(*OutAction)->DoAction = doAction;
	(*OutAction)->EquipmentColor = EquipmentColor;

}

FString UCActionData::GetLableName(class ACharacter* InOwnerCharacter, FString InName)
{
	FString str;
	str.Append(InOwnerCharacter->GetActorLabel());
	str.Append("_");
	str.Append(InName);
	str.Append("_");
	str.Append(GetName().Replace(L"DA_", L""));

	return str;
}



