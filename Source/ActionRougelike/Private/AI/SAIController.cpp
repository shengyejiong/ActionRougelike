// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(BehaviorTree), TEXT("BehaviorTree is nullptr, please assign in AIController."))//这个函数是一个断言函数，用于检查BehaviorTree是否为nullptr。如果BehaviorTree为nullptr，则会输出一条错误消息，提示开发者在AIController中分配BehaviorTree。这个函数的作用是确保BehaviorTree被正确设置，以避免在运行时出现错误。
	{
		RunBehaviorTree(BehaviorTree);
	}


	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);//这个函数是获取玩家的Pawn，参数0表示获取第一个玩家的Pawn。这个函数返回一个APawn类型的指针，可以用来控制玩家角色或者进行其他操作。
	//if (MyPawn)
	//{
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	//}
}
