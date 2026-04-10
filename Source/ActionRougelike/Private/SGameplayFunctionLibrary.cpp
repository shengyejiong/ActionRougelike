// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"
#include <SAttributeComponent.h>

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}

	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))//如果击中的组件存在并且正在模拟物理
		{
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();//这里标准化是为了确保方向向量的长度为1，这样在计算冲量时就可以直接使用这个方向向量乘以一个固定的值来得到一个合适的冲量大小。
			//根据击中结果中的方向向量来计算一个反向的冲量，并且将这个冲量应用到被击中的组件上。这个冲量的大小可以根据需要进行调整，这里使用了一个固定的值300000.f作为示例。
			//这个可以实现击退效果，让被击中的组件在受到伤害后有一个物理上的反应，增加游戏的真实感和反馈感。
			HitComp->AddImpulseAtLocation(Direction * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}

		return true;
	}
	return false;
}
