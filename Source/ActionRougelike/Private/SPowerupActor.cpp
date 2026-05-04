// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SpphereComp");
	SphereComp->SetCollisionProfileName("Powerup");//设置碰撞预设为Powerup，Powerup预设需要在编辑器中设置
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);//设置MeshComp的碰撞为NoCollision，因为我们只需要SphereComp来处理碰撞
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;
	bIsActive = true;

	SetReplicates(true);//设置这个Actor可以被复制到客户端，这样客户端才能看到这个Actor
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	//这里可以添加一些逻辑，比如给玩家增加能力或者道具等
}

FText ASPowerupActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{

	return FText::GetEmpty();//返回一个空的文本，因为这个道具没有交互文本
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

void ASPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);

	RootComponent->SetVisibility(bIsActive, true);
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor, bIsActive);//注册bIsActive变量为需要网络同步的属性，这样当服务器修改bIsActive的值时，客户端也会收到更新
}
