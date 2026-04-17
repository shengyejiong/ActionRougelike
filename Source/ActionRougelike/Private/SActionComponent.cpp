// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"


// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for(TSubclassOf<USAction> ActionClass : DefaultActions)//遍历默认动作数组，给每个动作类添加一个动作对象
	{
		AddAction(GetOwner(), ActionClass);//调用AddAction函数，传入这个组件所属的角色和这个动作类
	}
	
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();//构造一个调试信息字符串，包含这个组件所属的角色的名字和当前激活的Tag的列表
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	//新建一个动作对象，传入这个组件和动作类
	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))//如果新建成功了，就把这个动作添加到动作数组中
	{
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

			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}

		}
	}

	return false;
}
