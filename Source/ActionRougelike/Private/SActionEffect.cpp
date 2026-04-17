// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"
#include "SActionComponent.h"


USActionEffect::USActionEffect()
{
	bAutoStart = true;//设置这个效果在角色生成时自动开始的，这样当角色生成时这个效果就会自动开始执行
}


void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;//创建一个定时器委托对象，这个对象可以绑定一个函数来在定时器触发时调用
		Delegate.BindUFunction(this, "StopAction", Instigator);//绑定StopAction函数到定时器委托对象上，这样当定时器触发时就会调用StopAction函数来结束效果

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);//设置一个定时器来控制效果持续时间的结束，传入定时器句柄、定时器委托对象、持续时间和是否循环

	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;//创建一个定时器委托对象，这个对象可以绑定一个函数来在定时器触发时调用
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);//绑定ExecutePeriodicEffect函数到定时器委托对象上，这样当定时器触发时就会调用ExecutePeriodicEffect函数来执行周期效果

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);//设置一个定时器来控制周期效果的触发，传入定时器句柄、定时器委托对象、周期时间和是否循环
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{

	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);//如果周期效果的定时器剩余时间小于一个很小的数值，说明周期效果即将触发了，这时候我们就先执行一次周期效果，这样就不会错过最后一次周期效果的触发了
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);//清除周期效果的定时器，这样就不会再触发周期效果了
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);//清除持续时间的定时器，这样就不会再触发效果结束了

	USActionComponent* Comp = GetOwningComponent();//获取这个效果所属的组件，这样我们就可以通过这个组件来访问角色的Tag或者其他属性
	if (Comp)
	{
		Comp->RemoveAction(this);//从组件中移除这个效果，这样就不会再对角色产生影响了
	}
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{

}
