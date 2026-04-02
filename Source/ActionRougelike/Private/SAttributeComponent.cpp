// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
	HealthMax = Health;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());// 这个函数的意思是调用ApplyHealthChange函数来减少血量，传入的参数是造成伤害的Actor和要减少的血量值，这里要减少的血量值是当前最大血量的负数，也就是说要把血量减少到0
}

bool USAttributeComponent::IsAlive() const//这里const表示这个函数不会修改类的成员变量，可以在任何情况下调用，包括常量对象和非常量对象
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor,float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;// 这个函数的意思是如果Owner（也就是这个组件所属的Actor）不能被伤害，那么就直接返回false，表示没有应用血量变化
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);// FMath::Clamp函数会将Health + Delta的结果限制在0.0f和HealthMax之间，确保血量不会超过最大值或者变成负数

	float ActualDelta = Health - OldHealth;// 计算实际的血量变化值，可能会因为Clamp函数的限制而与传入的Delta值不同

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);// 触发血量变化的事件，参数分别是：造成伤害的Actor（这里传入InstigatorActor），属性组件本身，当前血量和实际的血量变化值

	return ActualDelta != 0;// 返回是否有实际的血量变化，如果ActualDelta不等于0，说明血量发生了变化，返回true；否则返回false
}

float USAttributeComponent::GetHealth()
{
	return Health;
}


USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));// 这个函数的意思是从FromActor中获取USAttributeComponent类型的组件，如果FromActor没有这个组件或者组件类型不匹配，则返回nullptr
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);// 这个函数的意思是从Actor中获取USAttributeComponent类型的组件，如果Actor没有这个组件或者组件类型不匹配，则返回nullptr
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();// 这个函数的意思是调用AttributeComp的IsAlive函数来判断Actor是否存活，如果AttributeComp为nullptr，则返回false
	}
	return false;
}

