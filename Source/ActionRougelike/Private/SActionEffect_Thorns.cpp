// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"
#include "SAttributeComponent.h"
#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"


USActionEffect_Thorns::USActionEffect_Thorns()
{
	ReflectFraction = 0.2f;//反弹比例20%

	Duration = 0.0f;
	Period = 0.0f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	//开始监听
	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());//获取属性组件
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);//绑定属性组件的OnHealthChanged事件，当角色受到伤害时会触发这个事件，我们在这个事件中来实现反弹伤害的逻辑
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());//获取属性组件
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}

}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();//获取拥有这个效果的角色
	
	//只有造成伤害才会触发反弹伤害的逻辑
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// Round to nearest int to avoid 'ugly' numbers and tiny reflections
		// 四舍五入到最近的整数，避免出现一些不美观的数字和微小的反弹伤害
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;//如果反弹伤害为0，则不执行反弹伤害的逻辑
		}

		ReflectedAmount = FMath::Abs(ReflectedAmount);//取反弹伤害的绝对值，因为Delta是负数，所以需要取绝对值来得到正数的反弹伤害
	
		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	
	}
}