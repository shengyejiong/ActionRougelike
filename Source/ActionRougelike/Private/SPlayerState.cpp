// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"


void ASPlayerState::AddCredits(int32 Delta)
{
	//这个判断确保Delta是正数，如果不是正数，就直接返回，不进行任何操作
	if (!ensure(Delta > 0))
	{
		return;
	}

	Credits += Delta;//增加Credits的值

	OnCreditsChanged.Broadcast(this, Credits, Delta);//广播事件，通知监听者Credits的值发生了变化

}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	//这个判断确保Delta是正数，如果不是正数，就直接返回false，表示移除Credits失败
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	//这个判断确保Credits的值足够移除，如果Credits的值小于Delta，就直接返回false，表示移除Credits失败
	if (Credits < Delta)
	{
		return false;
	}
	
	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);//广播事件，通知监听者Credits的值发生了变化

	return true;
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	// 先判断目标是否存在
	if (SaveObject)
	{
		AddCredits(SaveObject->Credits);//把SaveObject中的Credits的值添加到当前PlayerState的Credits中
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		Credits = SaveObject->Credits;//从SaveObject中加载Credits的值
	}
}

void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);//广播事件，通知监听者Credits的值发生了变化，参数分别是：当前PlayerState对象，新的Credits值和变化的Delta值（新的Credits值减去旧的Credits值）
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

/**
 * @brief 生命周期网络同步属性注册函数
 * 引擎会自动调用此函数，获取该类中需要进行网络复制的变量列表
 * @param OutLifetimeProps 输出：需要同步的属性列表
 */
void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// 调用父类的同步注册逻辑（必须写）
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 核心：把 Credits 变量注册为“需要网络同步”
	DOREPLIFETIME(ASPlayerState, Credits);
}