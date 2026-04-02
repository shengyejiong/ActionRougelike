// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;// 这个变量是用来存储生成敌人的类的，TSubclassOf<AActor>是一个模板类，可以用来存储一个类的子类，这样就可以在蓝图中选择一个类来生成敌人

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;// 这个变量是用来存储生成敌人的环境查询的，UEnvQuery是一个环境查询类，可以用来查询游戏世界中的一些信息，比如生成敌人的位置

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;// 这个变量是用来存储生成敌人难度的曲线的，UCurveFloat是一个曲线类，可以用来存储一个曲线，这个曲线可以用来根据游戏时间来调整生成敌人的难度

	FTimerHandle TimerHandle_SpawnBots;// 这个变量是用来控制生成敌人的时间的，FTimerHandle是一个定时器句柄，可以用来控制定时器的开启和关闭

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;// 这个变量是用来控制生成敌人的时间间隔的，单位是秒.

	UFUNCTION()
	void SpawnBotTimerElapsed();// 这个函数用来生成敌人

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);// 这个函数用来处理环境查询的结果，当环境查询完成时会调用这个函数来处理查询结果
	//这个函数的参数分别是：查询实例，查询状态，查询实例是一个环境查询实例的包装类，可以用来获取查询结果，查询状态是一个枚举类型，表示查询的状态，比如成功、失败等

public:

	ASGameModeBase();

	virtual void StartPlay() override;//这个类不用beginplay，因为它是一个基类，所有的游戏模式都要继承它，所以在这个类里实现startplay就好了，其他的游戏模式只需要继承这个类就可以了

	UFUNCTION(Exec)
	void KillAll();
};
