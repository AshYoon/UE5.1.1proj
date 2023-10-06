#include "CActionData.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "CAction.h"

// 이런식으로 변수넣기 
void UCActionData::BeginPlay(ACharacter* InOwnerCharacter, UCAction** OutAction)
{
	FTransform transform;


	ACAttachment* attachment = NULL; // null 값으로 초기화 



	// 어떻게 보면 직업자체를 데이터 에셋으로 관리한다고 보면된다  
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
		   // Attachment의 overlap과 collision을 DoActionClass에 연결 
		   attachment->OnAttamentBeginOverlap.AddDynamic(doAction, &ACDoAction::OnAttachmentBeginOverlap);
		   attachment->OnAttamentEndOverlap.AddDynamic(doAction, &ACDoAction::OnAttachmentEndOverlap);

		   attachment->OnAttachmentCollision.AddDynamic(doAction, &ACDoAction::OnAttachmentCollision);
		   attachment->OffAttachmentCollision.AddDynamic(doAction, &ACDoAction::OffAttachmentCollision);
	   }


	}

	// 

	// 리턴할때 new object로 하는거지 action으로 
	//가져다가 쓸때마다 복사된다 , 실제 스탯은 우리가 안에서 만들어서 정하고 , 이거 기반으로 DA 만들어서 정하니깐
	// 애를 호출하는건 Action Component 

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



