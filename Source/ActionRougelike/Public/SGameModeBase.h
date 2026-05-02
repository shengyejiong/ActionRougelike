// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class USSaveGame;
class UDataTable;
class USMonsterData;

// DataTable Row for spawning monsters in game mode - 数据表行，用于在游戏模式中生成怪物
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 10.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USMonsterData* MonsterData;// 这个变量是用来存储生成敌人的数据的，USMonsterData是一个数据资产类，可以用来存储一些数据，比如生成敌人的属性等
	//TSubclassOf<AActor> MonsterClass;// 这个变量是用来存储生成敌人的类的，TSubclassOf<AActor>是一个模板类，可以用来存储一个类的子类，这样就可以在蓝图中选择一个类来生成敌人

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;// 这个变量是用来存储生成敌人的权重的，权重越大，生成这个敌人的概率就越大，这样就可以根据不同的权重来控制生成不同类型的敌人的概率

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;// 这个变量是用来存储生成敌人的成本的，成本越高，生成这个敌人的代价就越大，这样就可以根据不同的成本来控制生成不同类型的敌人的数量，比如说生成一个强大的敌人需要更多的成本，那么就可以限制生成这个强大的敌人的数量，从而增加游戏的挑战性

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;// 这个变量是用来存储杀死这个敌人后获得的奖励的，奖励越高，杀死这个敌人后获得的积分就越多，这样就可以根据不同的奖励来控制玩家杀死不同类型的敌人的动机，比如说杀死一个强大的敌人可以获得更多的奖励，那么玩家就会更有动力去杀死这个强大的敌人，从而增加游戏的乐趣
};

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	FString SlotName;

	UPROPERTY()
	USSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;// 这个变量是用来存储生成敌人的数据表的，UDataTable是一个数据表类，可以用来存储一些数据，比如生成敌人的属性等

	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//TSubclassOf<AActor> MinionClass;// 这个变量是用来存储生成敌人的类的，TSubclassOf<AActor>是一个模板类，可以用来存储一个类的子类，这样就可以在蓝图中选择一个类来生成敌人

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;// 这个变量是用来存储生成敌人的环境查询的，UEnvQuery是一个环境查询类，可以用来查询游戏世界中的一些信息，比如生成敌人的位置

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;// 这个变量是用来存储生成敌人难度的曲线的，UCurveFloat是一个曲线类，可以用来存储一个曲线，这个曲线可以用来根据游戏时间来调整生成敌人的难度

	FTimerHandle TimerHandle_SpawnBots;// 这个变量是用来控制生成敌人的时间的，FTimerHandle是一个定时器句柄，可以用来控制定时器的开启和关闭

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;// 这个变量是用来控制生成敌人的时间间隔的，单位是秒.

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	UEnvQuery* PowerupSpawnQuery;// 这个变量是用来存储生成道具的环境查询的，UEnvQuery是一个环境查询类，可以用来查询游戏世界中的一些信息，比如生成道具的位置

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TArray<TSubclassOf<AActor>> PowerupClasses;// 这个变量是用来存储生成道具的类的，TArray<TSubclassOf<AActor>>是一个数组，可以用来存储多个类的子类，这样就可以在蓝图中选择多个类来生成道具

	UFUNCTION()
	void SpawnBotTimerElapsed();// 这个函数用来生成敌人

	UFUNCTION()
	void OnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);// 这个函数用来处理环境查询的结果，当环境查询完成时会调用这个函数来处理查询结果
	//这个函数的参数分别是：查询实例，查询状态，查询实例是一个环境查询实例的包装类，可以用来获取查询结果，查询状态是一个枚举类型，表示查询的状态，比如成功、失败等

	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);//这个函数用来处理玩家重生的事件，当一个玩家被杀死后会调用这个函数来处理玩家重生的逻辑，比如生成玩家角色等

public:

	//标记为virtual的函数可以被子类重写，这样子类就可以实现自己的逻辑来处理演员被杀死的事件，而不需要修改基类的代码，这样就可以提高代码的复用性和可维护性。
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);//这个函数用来处理演员被杀死的事件，当一个演员被杀死时会调用这个函数来处理相关逻辑，比如增加得分、生成掉落物等

	ASGameModeBase();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;//这个类不用beginplay，因为它是一个基类，所有的游戏模式都要继承它，所以在这个类里实现startplay就好了，其他的游戏模式只需要继承这个类就可以了

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(Exec)
	void KillAll();


	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int DesiredPowerupCount;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float RequiredPowerupDistance;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

};
