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
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
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

	//SetReplicates(true);//设置这个Actor可以被复制到客户端，这样客户端才能看到这个Actor
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}


void ASProjectileBase::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || OtherActor == GetInstigator())
	{
		return;
	}

	if (bDestroyed)
	{
		SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);//当投射物发生碰撞时，禁用碰撞以避免重复触发碰撞事件

		MoveComp->StopMovementImmediately();//立即停止投射物的移动
		EffectComp->Deactivate();//停用投射物的粒子效果
		if (AudioComp)
		{
			AudioComp->FadeOut(0.15f, 0.0f);//淡出投射物的音效，持续时间为0.15秒，最终音量为0
		}

		Explode();
	}

}


void ASProjectileBase::Explode_Implementation()
{
	// 通过检查来确保当前对象没有被标记为即将销毁（PendingKill）。如果对象已经被标记为即将销毁，说明它正在被销毁过程中，无法继续执行后续的操作。
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
		}

		SetLifeSpan(DestroyDelay);
	}
}


void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}


