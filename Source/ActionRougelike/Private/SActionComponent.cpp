// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"
#include "../ActionRougelike.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"


DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_STANFORD);//这个宏是用来声明一个性能统计的，传入三个参数，第一个参数是这个统计的名字，第二个参数是这个统计的显示名字，第三个参数是这个统计所属的组，这样我们就可以在性能分析工具中看到这个统计的信息了，在这个函数中，我们会在调用StartActionByName函数的时候记录这个统计，这样我们就可以知道这个函数的性能表现了

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);//这个函数是用来设置这个组件在网络上是否需要被复制的，传入true表示这个组件需要被复制，这样当这个组件所属的角色在服务器上发生变化时，这个组件的状态也会同步到所有客户端，这对于一些需要在网络上保持一致的组件来说是非常重要的，比如这个动作组件，我们希望当一个角色执行一个动作时，这个动作的状态能够正确地同步到所有客户端，这样所有玩家都能看到这个动作的效果，所以我们需要设置这个组件为需要被复制的
}




// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// 保证只在服务器上执行，因为我们希望只有服务器能够控制动作的执行，客户端只能通过服务器来同步动作的状态
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> ActionClass : DefaultActions)//遍历默认动作数组，给每个动作类添加一个动作对象
		{
			AddAction(GetOwner(), ActionClass);//调用AddAction函数，传入这个组件所属的角色和这个动作类
		}
	}

	
}

void USActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<USAction*> ActionsCopy = Actions;//创建一个动作数组的副本，包含当前动作数组中的所有动作对象，这样我们就可以在遍历这个副本的时候安全地修改原来的动作数组了，如果我们直接在遍历原来的动作数组的时候修改它，可能会导致一些问题，比如我们在遍历的时候移除了一些动作对象，这样就会导致遍历的过程中出现一些错误或者崩溃，所以我们需要创建一个副本来避免这些问题
	for (USAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());//如果这个动作对象存在，并且正在运行中，就调用这个动作的停止函数，传入这个组件所属的角色作为参数，这样就可以确保当这个组件被销毁的时候，所有正在运行的动作都能够正确地停止了，这样就不会导致一些问题，比如当一个角色被销毁的时候，它的动作对象还在运行中，这样可能会导致一些错误或者崩溃，所以我们需要确保当这个组件被销毁的时候，所有正在运行的动作都能够正确地停止了
		}
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();//构造一个调试信息字符串，包含这个组件所属的角色的名字和当前激活的Tag的列表
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// 遍历动作数组，给每个动作对象添加一个调试信息字符串，包含这个动作的名字、是否正在运行以及这个动作所属的组件的名字
	for (USAction* Action : Actions)
	{
		// 如果这个动作正在运行中，就把这个动作的名字显示在屏幕上，颜色为蓝色，否则颜色为白色
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s ; IsRunning: %s "), *GetNameSafe(GetOwner()), *GetNameSafe(Action), Action->IsRunning() ? TEXT("true") : TEXT("false"));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);

	}

}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	// 客户端跳过
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempted to AddAction, [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}


	//新建一个动作对象，传入这个组件和动作类
	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))//如果新建成功了，就把这个动作添加到动作数组中
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);
		
		//如果这个动作设置为自动开始，并且这个动作可以被执行，那么就让这个动作开始执行，传入Instigator作为执行这个动作的角色
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	//这个判断是为了确保我们要移除的这个动作对象存在，并且这个动作对象没有在运行中，如果这个动作对象正在运行中，我们就不应该移除它，因为这样可能会导致一些问题，比如这个动作对象正在执行一些逻辑或者触发一些事件，如果我们在这个时候移除它，就可能会导致一些错误或者崩溃，所以我们需要确保这个动作对象没有在运行中才可以安全地移除它
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);//从动作数组中移除这个动作对象
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);//这个宏是用来记录这个函数的性能统计的，传入我们之前声明的那个统计的名字，这样当这个函数被调用的时候，就会记录这个统计的信息了，这样我们就可以在性能分析工具中看到这个函数的性能表现了

	for (USAction* Action : Actions)//遍历动作数组，找到名字和传入的名字一样的动作
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;//如果这个动作不能被执行，就跳过这个动作继续找下一个动作
			}

			// 是客户端吗？如果是客户端，就调用服务器函数来让服务器执行这个动作，这样可以保证这个动作的状态能够正确地同步到所有客户端，如果我们直接在客户端执行这个动作，可能会导致一些问题，比如这个动作的状态只能在这个客户端上看到，而其他客户端看不到，这样就会导致一些不一致的情况，所以我们需要让服务器来执行这个动作，这样所有客户端都能看到这个动作的效果
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

			// 记录一个书签，包含这个动作的名字，这样我们就可以在性能分析工具中看到这个书签的信息了，这对于调试和优化这个函数的性能来说是非常有帮助的
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}


USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass) const
{
	if (!ActionClass)
	{
		return nullptr;
	}

	for (USAction* Action : Actions)
	{
		if (Action && Action->GetClass() == ActionClass)
		{
			return Action;
		}
	}

	return nullptr;
}


bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}

				Action->StopAction(Instigator);
				return true;
			}

		}
	}

	return false;
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action : Actions)
	{
		// 如果这个动作对象存在，就调用Channel->ReplicateSubobject函数来复制这个动作对象，传入这个动作对象、Bunch和RepFlags作为参数，这样就可以确保这个动作对象的状态能够正确地同步到所有客户端
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

/*
	这个函数为什么不需要在头文件主动声明
	是因为一旦有元素被标记为replicated，函数声明就会在"SActionComponent.generated.h"中主动创建
*/
void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}