// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"


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

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

