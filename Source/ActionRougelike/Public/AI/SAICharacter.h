// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;//这个类是一个组件，提供了感知功能，可以让AI角色感知周围的环境和其他角色。
class USAttributeComponent;//这个类是一个组件，提供了属性功能，可以让AI角色拥有一些属性，比如生命值、攻击力等。

UCLASS()
class ACTIONROUGELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASAICharacter();

protected:

	void SetTargetActor(AActor* NewTarget);//这个函数是用来设置目标角色的，可以在行为树中调用这个函数来更新AI角色的目标。


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	virtual void PostInitializeComponents() override;//这个函数是在组件初始化完成后调用的，可以在这里绑定事件或者进行其他初始化操作。

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
