// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


ASDashProjectile::ASDashProjectile()
{
	TeleportDelay = 0.2f;//设置一个默认的传送延迟时间
	DetonateDelay = 0.2f;//设置一个默认的爆炸延迟时间

	MoveComp->InitialSpeed = 6000.0f;//设置一个较高的初始速度，使得投射物能够快速飞行

}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDashProjectile::Explode, DetonateDelay);//设置一个定时器，在DetonateDelay秒后调用Explode函数
}


void ASDashProjectile::Explode_Implementation()
{

	//如果爆炸函数被调用了，就先清除定时器，避免重复调用
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();//停用特效组件，停止发出粒子效果

	MoveComp->StopMovementImmediately();//立即停止投射物的移动
	SetActorEnableCollision(false);//禁用碰撞，使得投射物不再与其他物体发生碰撞

	FTimerHandle TimerHandle_DelayedTeleport;//创建一个新的定时器句柄，用于传送延迟
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);//设置一个定时器，在TeleportDelay秒后调用TeleportInstigator函数

	//跳过基础实现，因为它会摧毁投射物，而我们想在传送后再销毁它
	//Super::Explode_Implementation();
}

void ASDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();//获取投射物的生成者，也就是玩家角色
	if (ensure(ActorToTeleport))
	{
		//保持生成者的旋转，否则在传送后可能会面朝错误的方向，导致玩家角色的控制感变差
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->K2_GetActorRotation(), false, false);//将生成者传送到投射物当前的位置，保持原有的旋转，禁用传送时的碰撞检测和物理模拟
	}
}