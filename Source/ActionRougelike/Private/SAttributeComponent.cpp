// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

	Health = 100;
}

bool USAttributeComponent::IsAlive() const//这里const表示这个函数不会修改类的成员变量，可以在任何情况下调用，包括常量对象和非常量对象
{
	return Health > 0.0f;
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);// 这里的Broadcast函数会调用所有绑定到OnHealthChanged委托的函数，参数分别是：造成伤害的Actor（这里暂时传入nullptr），属性组件本身，当前血量和血量变化值

	return true;
}



