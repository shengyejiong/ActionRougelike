// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API USBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;//这个函数是当这个任务被执行时调用的，返回值表示任务的结果，可以是成功、失败或正在进行中。


protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpread;//这个属性表示子弹的最大散布范围，可以在编辑器中设置为一个浮点数，表示子弹的散布角度或距离。

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;//这个属性是一个类的子类，表示要生成的子弹的类型，可以在编辑器中设置为具体的子弹类。


public:

	USBTTask_RangedAttack();
};
