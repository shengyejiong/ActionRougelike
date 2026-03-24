// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);//这个函数是获取玩家的Pawn，参数0表示获取第一个玩家的Pawn。这个函数返回一个APawn类型的指针，可以用来控制玩家角色或者进行其他操作。
	//if (MyPawn)
	//{
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	//}
}
