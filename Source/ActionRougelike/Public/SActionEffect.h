// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"


/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:

	void StartAction_Implementation(AActor* Instigator) override;//重写StartAction函数，在这个函数中我们会设置定时器来控制周期效果的触发和效果持续时间的结束

	void StopAction_Implementation(AActor* Instigator) override;//重写StopAction函数，在这个函数中我们会清除定时器来停止周期效果的触发

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;//设置效果持续时间
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;//设置效果的周期时间，比如每隔1秒钟就会触发一次效果

	FTimerHandle PeriodHandle;//设置一个定时器句柄，用来控制周期效果的触发
	FTimerHandle DurationHandle;//设置一个定时器句柄，用来控制效果持续时间的结束

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);//这个函数是用来执行周期效果的，传入一个角色作为参数，这个角色就是效果的施加者

	virtual void ExecutePeriodicEffect_Implementation(AActor* Instigator);

public:

	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetTimeRemaining() const;//这个函数是用来获取这个动作剩余的时间的，返回一个浮点数表示剩余的时间

	USActionEffect();

};
