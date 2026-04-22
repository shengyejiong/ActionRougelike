// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include <ActionRougelike/ActionRougelike.h>



bool USAction::CanStart_Implementation(AActor* Instigator) const
{
	if (IsRunning())
	{
		return false;//如果这个动作正在运行了，就不能再执行一次，所以返回false
	}

	USActionComponent* Comp = GetOwningComponent();//调用GetOwningComponent函数获取这个动作所属的组件，这样我们就可以通过这个组件来访问角色的Tag或者其他属性

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* Comp = GetOwningComponent();//调用GetOwningComponent函数获取这个动作所属的组件，这样我们就可以通过这个组件来访问角色的Tag或者其他属性
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);//在动作开始时，将GrantsTags中的Tag添加到组件的ActiveGameplayTags中，这样角色就会获得这些Tag的效果

	bIsRunning = true;//将bIsRunning设置为true，表示这个动作正在运行
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	ensureAlways(bIsRunning);//确保这个动作正在运行，如果不是的话就会在编辑器中弹出一个错误提示

	USActionComponent* Comp = GetOwningComponent();//调用GetOwningComponent函数获取这个动作所属的组件，这样我们就可以通过这个组件来访问角色的Tag或者其他属性
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);//在动作停止时，将GrantsTags中的Tag从组件的ActiveGameplayTags中移除，这样角色就会失去这些Tag的效果

	bIsRunning = false;//将bIsRunning设置为false，表示这个动作已经停止运行
}

UWorld* USAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>|外界在通过NewObject<T>创建对象时设置

	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());//尝试将外部对象转换为组件类型，如果成功则返回组件所在的世界对象，否则返回nullptr
	if (Comp)
	{
		return Comp->GetWorld();//如果转换成功，则调用组件的GetWorld函数获取世界对象并返回
	}

	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());//尝试将外部对象转换为USActionComponent类型，如果成功则返回组件指针，否则返回nullptr
}

bool USAction::IsRunning() const
{
	return bIsRunning;//返回bIsRunning的值，表示这个动作是否正在运行
}