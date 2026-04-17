// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <SGameplayFunctionLibrary.h>
#include "SActionComponent.h"
#include "SActionEffect.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
		第一种碰撞设置:直接修改碰撞响应
	*/
	//SphereComp->SetCollisionObjectType(ECC_WorldDynamic);//设置碰撞类型
	//SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);//忽略所有碰撞
	//SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);//与Pawn发生重叠时触发碰撞
	/*
		第二种碰撞设置:使用预设的碰撞配置文件
	*/
	SphereComp->SetCollisionProfileName("Projectile");

	//MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	//MovementComp->InitialSpeed = 1000.0f;
	//MovementComp->bRotationFollowsVelocity = true;
	//MovementComp->bInitialVelocityInLocalSpace = true;


}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasAppliedHit)
	{
		return;
	}

	if (OtherActor && OtherActor != GetInstigator())
	{
		//可以使用static来创建一个静态的标签对象，这样在函数的多次调用中都会使用同一个标签对象，节省资源和提高性能。
		//static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("status.Parrying");

		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MoveComp->Velocity = -MoveComp->Velocity;//反弹

			SetInstigator(Cast<APawn>(OtherActor));//改变伤害来源为反弹后的角色
			return;//如果被格挡了，就不继续往下执行了，不会造成伤害
		}

		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			bHasAppliedHit = true;
			SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			MoveComp->StopMovementImmediately();
			Explode();

			if (ActionComp)
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}

	}
}

