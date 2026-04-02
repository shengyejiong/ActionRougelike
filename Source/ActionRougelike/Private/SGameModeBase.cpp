// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;//这个变量的默认值是2秒，也就是说每隔2秒钟就会生成一个敌人
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);//这个函数的意思是每隔SpawnTimerInterval秒钟就调用一次SpawnBotTimerElapsed函数，直到游戏结束
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)//这个函数的意思是遍历当前场景中的所有ASAICharacter类型的演员，也就是敌人
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);//这个函数的意思是获取敌人身上的属性组件，也就是存储敌人生命值的组件
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);//这个函数的意思是调用属性组件的Kill函数来杀死敌人，参数是这个类，也就是游戏模式类，可以用来记录杀死敌人的信息，比如得分等
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	/*
	* 把这段逻辑放到这里是因为我们希望生成敌人的逻辑能够定期执行，而不是只在某个事件发生时才执行。
	*/
	int32 NrOfAliveBots = 0;//这个变量的意思是存储当前场景中存活的敌人数量
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)//这个函数的意思是遍历当前场景中的所有ASAICharacter类型的演员，也就是敌人
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);//这个函数的意思是获取敌人身上的属性组件，也就是存储敌人生命值的组件
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NrOfAliveBots);//这个函数的意思是输出当前场景中存活的敌人数量到日志中，方便调试和查看。%i是一个占位符，表示输出一个整数，NrOfAliveBots是要输出的整数变量

	float MaxBotCount = 10.0f;//这个变量的意思是存储最大敌人数量的阈值，也就是说当场景中敌人数量超过这个值时，就不再生成新的敌人
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);//这个函数的意思是根据游戏时间来调整生成敌人的难度，DifficultyCurve是一个曲线类，可以用来存储一个曲线，这个曲线可以用来根据游戏时间来调整生成敌人的难度
	}


	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity, skipping bot spawn"));//这个函数的意思是输出当前场景中敌人数量已经达到最大值，跳过生成敌人的日志信息，方便调试和查看
		return;//这个函数的意思是如果当前场景中敌人数量超过最大值，那么就直接返回，不处理查询结果
	}
	 
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	//这个函数的意思是运行一个环境查询，查询的类型是SpawnBotQuery，查询的结果会随机选择最好的5%的结果，然后调用SpawnBotQueryFinished函数来处理查询结果
	//这个函数的参数分别是：查询的上下文，这个类，查询的结果处理函数，查询的运行模式，查询的参数

	if (ensure(QueryInstance))//这个函数的意思是检查查询实例是否有效
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);//这个函数的意思是当查询完成时，调用OnQueryCompleted函数来处理查询结果
	}

}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{

	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"));
		return;//这个函数的意思是如果查询状态不是成功，那么就直接返回，不处理查询结果
	}


	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();//这个函数的意思是获取查询结果中的位置，查询结果可能是一个数组，所以要用一个数组来存储这些位置

	if (Locations.IsValidIndex(0))//这个函数的意思是如果查询结果中有位置，那么就生成一个敌人
	{

		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0] + FVector(0,0,80), FRotator::ZeroRotator);//这个函数的意思是生成一个敌人，敌人的类是MinionClass，生成的位置是查询结果中的第一个位置，生成的旋转是零旋转

		DrawDebugSphere(GetWorld(), Locations[0] + FVector(0, 0, 80), 50.0f, 20, FColor::Blue, false, 60.0f);//绘制一个调试球体在生成敌人的位置，半径为50，分段数为20，颜色为蓝色，不持久化，持续时间为60秒，这个函数可以用来调试和可视化生成敌人的位置
	}
}

