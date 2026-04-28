// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"



void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);// 一旦玩家控制的Pawn发生变化，就会调用SetPawn函数，并通过OnPawnChanged委托广播这个变化，通知所有绑定了这个委托的函数或蓝图事件，以便它们可以根据新的Pawn进行相应的处理。
}
