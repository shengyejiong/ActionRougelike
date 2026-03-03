// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	ForceComp->SetAutoActivate(false);//默认情况下不激活

	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;//设置半径和冲量强度

	ForceComp->bImpulseVelChange = true;//设置冲量不受质量影响

	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);//设置影响的碰撞通道


}

void ASExplosiveBarrel::PostInitializeComponents()
{
	//别忘了调用父类的PostInitializeComponents函数，否则可能会导致一些组件没有正确初始化
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);//绑定组件被击中事件，当组件被击中时调用OnActorHit函数
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();//当组件被击中时，触发冲量
}



