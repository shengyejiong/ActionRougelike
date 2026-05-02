// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include <ActionRougelike/ActionRougelike.h>
#include "Net/UnrealNetwork.h"

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;//将传入的组件指针赋值给ActionComp，这样这个动作就知道它所属的组件了
}

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
	UE_LOG(LogTemp, Log, TEXT("Started: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* Comp = GetOwningComponent();//调用GetOwningComponent函数获取这个动作所属的组件，这样我们就可以通过这个组件来访问角色的Tag或者其他属性
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);//在动作开始时，将GrantsTags中的Tag添加到组件的ActiveGameplayTags中，这样角色就会获得这些Tag的效果

	RepData.bIsRunning = true;//将bIsRunning设置为true，表示这个动作正在运行
	RepData.Instigator = Instigator;//将Instigator存储在RepData中，这样我们就可以在网络中同步这个信息

	//只有在服务器上才会记录这个动作开始的时间，因为只有服务器才是这个动作的权威者，客户端只是从服务器同步这个状态
	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;//记录这个动作开始的时间，这样我们就可以在后续的逻辑中根据这个时间来做一些处理，比如计算这个动作已经持续了多久等等
	}

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);//在动作开始时，广播一个事件，通知其他系统这个动作已经开始了，这样其他系统就可以根据这个事件来做一些反应，比如播放一个动画或者触发一个特效等等
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);//确保这个动作正在运行，如果不是的话就会在编辑器中弹出一个错误提示

	USActionComponent* Comp = GetOwningComponent();//调用GetOwningComponent函数获取这个动作所属的组件，这样我们就可以通过这个组件来访问角色的Tag或者其他属性
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);//在动作停止时，将GrantsTags中的Tag从组件的ActiveGameplayTags中移除，这样角色就会失去这些Tag的效果

	RepData.bIsRunning = false;//将bIsRunning设置为false，表示这个动作已经停止运行
	RepData.Instigator = Instigator;//将Instigator存储在RepData中，这样我们就可以在网络中同步这个信息

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);//在动作停止时，广播一个事件，通知其他系统这个动作已经停止了，这样其他系统就可以根据这个事件来做一些反应，比如停止一个动画或者触发一个特效等等
}

UWorld* USAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>|外界在通过NewObject<T>创建对象时设置
	AActor* Actor = Cast<AActor>(GetOuter());//尝试将外部对象转换为组件类型，如果成功则返回组件所在的世界对象，否则返回nullptr
	if (Actor)
	{
		return Actor->GetWorld();//如果转换成功，则调用组件的GetWorld函数获取世界对象并返回
	}

	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	//AActor* Actor = Cast<AActor>(GetOuter());
	//return Actor->GetComponentByClass(USActionComponent::StaticClass);
	 
	return ActionComp;//尝试将外部对象转换为USActionComponent类型，如果成功则返回组件指针，否则返回nullptr
}

//这个函数是用来处理bIsRunning属性在网络中同步时的变化的，当bIsRunning发生变化时，这个函数会被调用，如果bIsRunning变为true，就调用StartAction函数来开始这个动作，如果bIsRunning变为false，就调用StopAction函数来停止这个动作，这样就可以保证这个动作的状态能够正确地同步到所有客户端
void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}


bool USAction::IsRunning() const
{
	return RepData.bIsRunning;//返回bIsRunning的值，表示这个动作是否正在运行
}


/*
	这个函数是用来告诉UE这个类中有哪些属性需要在网络中同步的
	在这个函数中，我们调用了DOREPLIFETIME宏来告诉UE需要同步bIsRunning这个属性
	这样当bIsRunning发生变化时，UE就会自动将这个变化同步到所有客户端
	并且在客户端上调用OnRep_IsRunning函数来处理这个变化
*/
void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, TimeStarted);
	DOREPLIFETIME(USAction, ActionComp);
}
