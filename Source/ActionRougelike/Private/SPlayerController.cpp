// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "Blueprint/UserWidget.h"


/**
 * @brief 暂停菜单开关函数（打开暂停界面）
 * 功能：创建暂停菜单UI，显示到屏幕，显示鼠标并锁定视角，仅允许操作UI
 */
void ASPlayerController::TogglePauseMenu()
{
	// 判断：暂停菜单已存在且正在显示 → 执行关闭
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;

	}

	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		// 将UI添加到视口，ZOrder=100确保显示在最上层
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		// 设置输入模式为仅UI，玩家无法控制角色移动/视角
		SetInputMode(FInputModeUIOnly());
	}
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASPlayerController::TogglePauseMenu);// 将输入事件绑定到TogglePauseMenu函数，当玩家按下指定的键（例如P键）时，调用TogglePauseMenu函数来切换暂停菜单的显示状态。
}

void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);// 一旦玩家控制的Pawn发生变化，就会调用SetPawn函数，并通过OnPawnChanged委托广播这个变化，通知所有绑定了这个委托的函数或蓝图事件，以便它们可以根据新的Pawn进行相应的处理。
}

void ASPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();// 当玩家控制器进入游戏状态时，调用BlueprintBeginPlayingState函数，这个函数是一个蓝图可实现事件，允许在蓝图中定义进入游戏状态时的行为，例如初始化UI、设置初始视角等。
}

void ASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateReceived.Broadcast(PlayerState);// 当玩家状态发生变化时，调用OnPlayerStateReceived委托广播这个变化，通知所有绑定了这个委托的函数或蓝图事件，以便它们可以根据新的玩家状态进行相应的处理。
}

