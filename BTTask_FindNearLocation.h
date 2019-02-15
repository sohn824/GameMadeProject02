// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindNearLocation.generated.h"

/**
 * 
 */
UCLASS()
class GAMEMADEPROJECT_API UBTTask_FindNearLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_FindNearLocation();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//ExecuteTask : �½�ũ�� ������ �� �ݵ�� �����ؾ� �ϴ� �Լ�
	//�� �Լ��� �� �� �ϳ��� ���� ��ȯ�ؾ� �Ѵ�
	//1. Failed : �½�ũ ���� ����
	//2. Succeeded : �½�ũ ���� ����
	//3. InProgress : �½�ũ�� ��� ���� ���̴�. ���� ����� ���߿� �˷��� �����̴�.
	//�����̺�� Ʈ���� Sequence �������� �ڽſ� ���� �½�ũ�� ������ �� ���� ��� �����Ѵ�
};
