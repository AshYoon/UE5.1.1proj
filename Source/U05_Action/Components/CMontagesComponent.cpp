#include "Components/CMontagesComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"




UCMontagesComponent::UCMontagesComponent()
{




}



void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();


	CheckNull(DataTable);

	TArray< FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);

	//for (const FMontageData* data : datas)
	//{
	//   if (!!data)// �����Ͱ��ִٸ� 
	//      CLog::Print(data->AnimMontage->GetPathName()); // ��� ��� 
	//}

	for (int32 i = 0; i < (int32)EStateType::Max; i++) // ��ü ũ�� ���������� 
	{
		for (FMontageData* data : datas) 
		{
			if ((EStateType)i == data->Type) // ���ڸ� enum���� �ٲ㼭 Type�̶� ������ ������ �־��ֱ�
			{
				Datas[i] = data;

				continue;
			}
		}//for(data)
	}//for(i)
	
}

void UCMontagesComponent::PlayRoll()
{
	PlayAnimMontage(EStateType::Roll);
}

void UCMontagesComponent::PlayBackstep()
{
	PlayAnimMontage(EStateType::Backstep);
}

void UCMontagesComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InState)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FMontageData* data = Datas[(int32)InState];
	if (!!data)
	{
		if (!!data->AnimMontage)
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);// �츮�� ������ ���̺� ������ �ɼ� 
	}
}

void UCMontagesComponent::PlayDead()
{
	PlayAnimMontage(EStateType::Dead);
}


