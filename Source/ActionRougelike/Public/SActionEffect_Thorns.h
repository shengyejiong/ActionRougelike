// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"

class USAttributeComponent;

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()
	

public:

	USActionEffect_Thorns();

	void StartAction_Implementation(AActor* Instigator) override;//重写StartAction函数，在这个函数中我们会设置定时器来控制周期效果的触发和效果持续时间的结束

	void StopAction_Implementation(AActor* Instigator) override;//重写StopAction函数，在这个函数中我们会清除定时器来停止周期效果的触发


protected:
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float ReflectFraction;//设置效果持续时间

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);



};
