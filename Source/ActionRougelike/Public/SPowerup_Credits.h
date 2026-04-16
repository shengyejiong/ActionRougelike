// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASPowerup_Credits : public ASPowerupActor
{
	GENERATED_BODY()

public:

	ASPowerup_Credits();//构造函数声明

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	float CreditsAmount;//这是一个浮点数变量，用于存储这个Powerup提供的Credits数量

	void Interact_Implementation(APawn* InstigatorPawn) override;//这是一个函数声明，表示当玩家与这个Powerup交互时会调用这个函数来实现具体的交互逻辑
	
};
