// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}



void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);//将生成者转换为角色类型，如果转换失败则返回nullptr
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);//播放攻击动画

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);//在角色的手部位置生成一个特效，特效的位置和旋转都设置为零向量，这样特效就会跟随角色的手部移动和旋转

		FTimerHandle TimerHandle_AttackDelay;//定义一个定时器句柄
		FTimerDelegate Delegate;//定义一个定时器委托
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);//将委托绑定到当前对象的AttackDelay_Elapsed函数，并传入角色作为参数

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);//设置一个定时器，在AttackAnimDelay秒后调用委托，定时器不会循环

	}
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))//检查要生成的类是否有效，如果无效则输出错误信息并中断程序，但在发布版本中会被忽略，所以可以安全地使用来检查类是否有效
	{
		FVector HandleLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);//获取发射点位置

		FActorSpawnParameters SpawnParams;//生成参数
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//忽略碰撞直接生成
		SpawnParams.Instigator = InstigatorCharacter;//设置生成者为角色

		FCollisionShape Shape;//碰撞形状
		Shape.SetSphere(20.0f);//设置为一个半径为20的球形


		FCollisionQueryParams Params;//碰撞查询参数
		Params.AddIgnoredActor(InstigatorCharacter);//忽略自己

		FCollisionObjectQueryParams ObjParams;//碰撞对象查询参数
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);//查询动态物体
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);//查询静态物体
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);//查询角色

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();//从角色的视点位置开始

		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);//向摄像机朝向的方向发出一条长度为5000的射线

		FHitResult Hit;//碰撞结果

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))//如果射线与某个物体发生了碰撞
		{
			TraceEnd = Hit.ImpactPoint;//将射线的终点设置为碰撞点，这样生成的投射物就会朝向碰撞点飞行
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandleLocation).Rotator();//根据发射点位置和朝向计算生成的投射物的旋转

		FTransform SpawnTM = FTransform(ProjRotation, HandleLocation);//生成的投射物的变换
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);//生成投射物
	}

	StopAction(InstigatorCharacter);//结束动作
}
