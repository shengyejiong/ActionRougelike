// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
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
