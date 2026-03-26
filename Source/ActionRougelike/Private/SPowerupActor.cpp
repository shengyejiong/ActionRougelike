// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SpphereComp");
	SphereComp->SetCollisionProfileName("Powerup");//设置碰撞预设为Powerup，Powerup预设需要在编辑器中设置
	RootComponent = SphereComp;

	RespawnTime = 10.0f;

}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	//这里可以添加一些逻辑，比如给玩家增加能力或者道具等
}

void ASPowerupActor::ShowPowerup()
{
	SetPowerupState(true);//显示道具
}

void ASPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);//隐藏道具

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPowerupActor::ShowPowerup, RespawnTime);//设置计时器，在RespawnTime秒后调用ShowPowerup函数显示道具
}

void ASPowerupActor::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);//根据bNewIsActive参数设置碰撞状态


	RootComponent->SetVisibility(bNewIsActive, true);//根据bNewIsActive参数设置可见性，第二个参数表示是否包括子组件

}


