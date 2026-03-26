// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
 
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);//给组件添加一个事件，当组件发生碰撞时会调用ASProjectileBase::OnActorHit函数
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	MoveComp->bRotationFollowsVelocity = true;//设置投射物的旋转跟随其速度方向
	MoveComp->bInitialVelocityInLocalSpace = true;//设置投射物的初始速度在局部空间中
	MoveComp->ProjectileGravityScale = 0.0f;//设置投射物的重力缩放为0，表示不受重力影响
	MoveComp->InitialSpeed = 8000;

	ImpactShakeInnerRadius = 0.0f;
	ImpactShakeOuterRadius = 1500.0f;


}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}


void ASProjectileBase::Explode_Implementation()
{
	// 通过检查来确保当前对象没有被标记为即将销毁（PendingKill）。如果对象已经被标记为即将销毁，说明它正在被销毁过程中，无法继续执行后续的操作。
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		Destroy();
	}
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}


