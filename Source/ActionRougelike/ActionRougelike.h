// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/*
	这个函数是一个实用函数，用于在屏幕上显示调试信息
	参数说明：
	- WorldContext: 上下文对象，通常是一个Actor或Component，用于获取当前的世界（World）对象。
	- Msg: 要显示的消息字符串。
	- Color: 消息的颜色，默认为白色。
	- Duration: 消息显示的持续时间，默认为5秒。
*/
static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f)
{
	// 确保WorldContext对象有效，如果无效则直接返回
	if (!ensure(WorldContext))
	{
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	// 确保获取到的World对象有效，如果无效则直接返回
	if (!ensure(World))
	{
		return;
	}

	FString NetPrefix = World->IsNetMode(NM_Client) ? "[Client]" : "[Server]";// 根据网络模式添加前缀，区分是客户端还是服务器
	// 使用全局的GEngine对象来显示调试消息，如果GEngine有效，则调用AddOnScreenDebugMessage方法显示消息
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
	}
}