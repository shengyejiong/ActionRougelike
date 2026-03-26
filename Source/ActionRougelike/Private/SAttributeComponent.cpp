// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
	HealthMax = Health;
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

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);// FMath::Clamp函数会将Health + Delta的结果限制在0.0f和HealthMax之间，确保血量不会超过最大值或者变成负数

	float ActualDelta = Health - OldHealth;// 计算实际的血量变化值，可能会因为Clamp函数的限制而与传入的Delta值不同

	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);// 触发血量变化的事件，参数分别是：造成伤害的Actor（这里暂时传入nullptr），属性组件本身，当前血量和实际的血量变化值

	return ActualDelta != 0;// 返回是否有实际的血量变化，如果ActualDelta不等于0，说明血量发生了变化，返回true；否则返回false
}



