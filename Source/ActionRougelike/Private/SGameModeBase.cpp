// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include <SCharacter.h>
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "SGameplayInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SMonsterData.h"
#include "../ActionRougelike.h"
#include "SActionComponent.h"

//这个变量的意思是创建一个控制台变量，名字是su.SpawnBots，默认值是false，帮助信息是Enable spawning of bots via timer，这个变量的作用是用来控制是否启用定时生成敌人的功能的，如果这个变量的值是false，那么就不会启用定时生成敌人的功能了
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;//这个变量的默认值是2秒，也就是说每隔2秒钟就会生成一个敌人
	CreditsPerKill = 20;//这个变量的默认值是20，也就是说每杀死一个敌人就会获得20点积分

	DesiredPowerupCount = 10;//这个变量的默认值是10，也就是说场景中最多只能有10个道具，如果超过这个数量了，那么就不再生成新的道具了
	RequiredPowerupDistance = 2000;//这个变量的默认值是2000，也就是说生成道具时，新的道具必须与已有道具保持至少2000单位的距离

	PlayerStateClass = ASPlayerState::StaticClass();//这个变量的默认值是ASPlayerState类，也就是说每个玩家都会有一个ASPlayerState类的实例来存储玩家的信息，比如积分、等级等

	SlotName = "SaveGame01";//这个变量的默认值是SaveGame01，也就是说保存游戏数据时会使用这个名字来保存，如果需要保存多个游戏数据，可以在蓝图中修改这个变量的值来使用不同的名字来保存游戏数据
}

// 初始化游戏时调用这个函数，这个函数的参数分别是：地图名称，选项，错误信息，这个函数的作用是加载保存的游戏数据，如果有的话
void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);//这个函数的意思是每隔SpawnTimerInterval秒钟就调用一次SpawnBotTimerElapsed函数，直到游戏结束

	//如果PowerupClasses数组中有元素，那么就运行一个环境查询来生成道具，查询的类型是PowerupSpawnQuery，查询的结果会调用OnPowerupSpawnQueryCompleted函数来处理查询结果
	if (ensure(PowerupClasses.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);

		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerupSpawnQueryCompleted);
		}

	}
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{

	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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

	//这个控制台参数的意思是如果启用了定时生成敌人的功能，那么就继续执行生成敌人的逻辑，如果没有启用定时生成敌人的功能，那么就直接返回，不执行生成敌人的逻辑了，这样就可以通过控制台命令来控制是否启用定时生成敌人的功能了
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}

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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnBotQueryCompleted);//这个函数的意思是当查询完成时，调用OnQueryCompleted函数来处理查询结果
	}

}

void ASGameModeBase::OnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{

	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"));
		return;//这个函数的意思是如果查询状态不是成功，那么就直接返回，不处理查询结果
	}


	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();//这个函数的意思是获取查询结果中的位置，查询结果可能是一个数组，所以要用一个数组来存储这些位置

	if (Locations.IsValidIndex(0))//这个函数的意思是如果查询结果中有位置，那么就生成一个敌人
	{

		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			// 随机选择一行数据来生成敌人
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			AActor* NewBot = GetWorld()->SpawnActor<AActor>(SelectedRow->MonsterData->MonsterClass, Locations[0] + FVector(0, 0, 80), FRotator::ZeroRotator);//这个函数的意思是生成一个敌人，敌人的类是MinionClass，生成的位置是查询结果中的第一个位置，生成的旋转是零旋转
			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(SelectedRow->MonsterData)));

				USActionComponent* ActionComp = Cast<USActionComponent>(NewBot->GetComponentByClass(USActionComponent::StaticClass()));//这个函数的意思是获取敌人身上的动作组件，也就是存储敌人技能的组件
				if (ActionComp)
				{
					for (TSubclassOf<USAction> ActionClass : SelectedRow->MonsterData->Actions)
					{
						ActionComp->AddAction(NewBot, ActionClass);//这个函数的意思是给敌人添加技能，参数是敌人和技能类，可以用来生成技能实例并添加到敌人身上
					}
				}
			}
		}

	}
}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)//这个函数的意思是如果查询状态不是成功，那么就直接返回，不处理查询结果
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();//这个函数的意思是获取查询结果中的位置，查询结果可能是一个数组，所以要用一个数组来存储这些位置

	TArray<FVector> UsedLocations;//这个变量的意思是存储已经使用过的位置，也就是说生成道具时，新的道具必须与已有道具保持至少RequiredPowerupDistance单位的距离，如果新位置与已有位置的距离小于RequiredPowerupDistance，那么就不生成道具了

	int32 SpawnCounter = 0;//这个变量的意思是存储已经生成的道具数量，也就是说当生成的道具数量达到DesiredPowerupCount时，就不再生成新的道具了

	//这个函数的意思是当查询完成时，处理查询结果，生成道具，直到生成的道具数量达到DesiredPowerupCount或者没有可用的位置了为止
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);//这个函数的意思是随机选择一个位置的索引，范围是从0到查询结果中的位置数量减1

		FVector PickedLocation = Locations[RandomLocationIndex];//这个函数的意思是根据随机选择的位置索引，获取这个位置
		Locations.RemoveAt(RandomLocationIndex);//这个函数的意思是从查询结果中移除已经使用过的位置，这样就不会重复使用同一个位置了

		bool bValidLocation = true;//这个变量的意思是存储当前选择的位置是否有效，也就是说新的道具必须与已有道具保持至少RequiredPowerupDistance单位的距离，如果新位置与已有位置的距离小于RequiredPowerupDistance，那么就不生成道具了
		
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();//这个函数的意思是计算当前选择的位置与已有位置的距离，PickedLocation是当前选择的位置，OtherLocation是已有位置，Size函数是计算向量的长度，也就是距离

			if (DistanceTo < RequiredPowerupDistance)
			{
				bValidLocation = false;//这个函数的意思是如果当前选择的位置与已有位置的距离小于RequiredPowerupDistance，那么就把当前选择的位置标记为无效，这样就不会生成道具了
				break;
			}
		}

		if (!bValidLocation)
		{
			continue;//这个函数的意思是如果当前选择的位置无效，那么就跳过当前循环，继续随机选择下一个位置了
		}

		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);//这个函数的意思是随机选择一个道具类的索引，范围是从0到PowerupClasses数组中的元素数量减1
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];//这个函数的意思是根据随机选择的道具类索引，获取这个道具类

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);//这个函数的意思是生成一个道具，道具的类是随机选择的道具类，生成的位置是当前选择的位置，生成的旋转是零旋转

		UsedLocations.Add(PickedLocation);//这个函数的意思是把当前选择的位置添加到已经使用过的位置数组中，这样就可以保证新的道具与已有道具保持至少RequiredPowerupDistance单位的距离了
		SpawnCounter++;//这个函数的意思是增加已经生成的道具数量的计数器，这样就可以知道已经生成了多少个道具了，当生成的道具数量达到DesiredPowerupCount时，就不再生成新的道具了

	}

}


void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();//这个函数的意思是让玩家控制器放弃对当前角色的控制，这样就可以生成一个新的角色来控制了

		RestartPlayer(Controller);//这个函数的意思是重启玩家，也就是生成玩家角色，参数是玩家的控制器，可以用来生成玩家角色等
	}
}


void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		//这个函数的意思是创建一个定时器委托，当定时器到期时，调用RespawnPlayerElapsed函数来处理玩家重生的逻辑，参数是玩家的控制器，可以用来生成玩家角色等
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		//这个函数的意思是设置一个定时器，当定时器到期时，调用Delegate来处理玩家重生的逻辑，参数是RespawnDelay，也就是重生的延迟时间，单位是秒，最后一个参数表示定时器是否循环，这里设置为false，也就是说只会调用一次
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	APawn* KillerPawn = Cast<APawn>(Killer);//这个函数的意思是把杀死演员的角色转换成一个Pawn类型的角色，这样就可以获取杀死演员的玩家状态了

	if (KillerPawn)
	{
		ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();//这个函数的意思是获取杀死演员的玩家状态，也就是存储玩家信息的类，比如积分、等级等

		if (PS)
		{
			PS->AddCredits(CreditsPerKill);//这个函数的意思是给杀死演员的玩家增加积分，参数是每杀死一个敌人获得的积分数量
		}
	}

}


void ASGameModeBase::WriteSaveGame()
{
	// Iterate all player states,we don't have proper Id to match yet(requires Steam or EOS)
	// 译文：迭代所有玩家状态，我们还没有合适的ID来匹配（需要Steam或EOS）
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);//这个函数的意思是把玩家状态保存到当前的保存游戏数据中，参数是当前的保存游戏对象，可以用来存储玩家状态的信息，比如积分、等级等
			break;// single player only at this point - 单人游戏
		}
	}
	// 先清空旧数据
	CurrentSaveGame->SavedActors.Empty();

	// 遍历当前世界中的所有 Actor
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// 只保留实现了 USGameplayInterface 接口的 Actor（例如可交互物品、NPC 等）
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		// 为该 Actor 构造一份存档数据：记录名称和当前变换
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);//这个函数的意思是创建一个对象和名称作为字符串的代理存档，参数是内存写入器，第二个参数表示是否保存对象引用，这里设置为true，也就是说在序列化Actor时会把Actor的引用也保存下来，这样在加载游戏数据时就可以正确地恢复Actor的状态了
		Ar.ArIsSaveGame = true;

		Actor->Serialize(Ar);

		// 将数据添加到全局的存档对象中
		CurrentSaveGame->SavedActors.Add(ActorData);
	}


	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);//这个函数的意思是把当前的保存游戏数据保存到指定的槽位中，参数分别是保存游戏对象，槽位名称，用户索引，这里用户索引设置为0，表示默认用户
}



void ASGameModeBase::LoadSaveGame()
{
	// 如果存在保存游戏，那么就从保存游戏中加载游戏数据
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));

		// 遍历当前世界中的所有 Actor
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// 只保留实现了 USGameplayInterface 接口的 Actor（例如可交互物品、NPC 等）
			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				// 比对对象名字
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					// Convert binary array back into actor's variables

					Actor->Serialize(Ar);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);// 这个函数的意思是调用Actor的OnActorLoaded函数来处理Actor加载完成后的逻辑，这个函数是通过接口来调用的，也就是说只要Actor实现了USGameplayInterface接口，并且在接口中定义了OnActorLoaded函数，那么就可以在加载游戏数据时正确地调用这个函数来处理相关逻辑

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
	
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}


}
