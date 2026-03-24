// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"




EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();//获取AI控制器

	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());//获取AI控制的角色
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;//如果没有获取到角色，返回失败
		}

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");//获取角色的枪口位置
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));//从黑板中获取目标角色

		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;//如果没有获取到目标角色，返回失败
		}

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;//计算从枪口到目标的方向,方法为目标角色的位置减去枪口的位置，得到一个向量，表示从枪口指向目标的方向。
		FRotator MuzzleRotation = Direction.Rotation();//将方向向量转换为旋转，得到枪口应该朝向的旋转。

		FActorSpawnParameters Params;//创建一个结构体，用于指定生成子弹时的参数
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//设置生成子弹时的碰撞处理方式为始终生成

		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);//在世界中生成一个新的子弹，使用指定的类、位置、旋转和参数

		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;//如果生成子弹成功，返回成功，否则返回失败

	}

	return EBTNodeResult::Failed;
}
