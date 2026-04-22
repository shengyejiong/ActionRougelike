// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;//这个类是一个组件，提供了感知功能，可以让AI角色感知周围的环境和其他角色。
class USAttributeComponent;//这个类是一个组件，提供了属性功能，可以让AI角色拥有一些属性，比如生命值、攻击力等。
class UUserWidget;//这个类是一个用户界面小部件的基类，可以用来创建各种用户界面元素，比如血量条、技能图标等。
class USWorldUserWidget;//这个类是一个用户界面小部件的子类，提供了一些额外的功能，比如绑定到世界坐标、自动更新位置等，可以用来创建一些特殊的用户界面元素，比如血量条、名字标签等。
class USActionComponent;//这个类是一个组件，提供了动作功能，可以让AI角色拥有一些动作，比如攻击、移动等。

UCLASS()
class ACTIONROUGELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASAICharacter();

protected:


	USWorldUserWidget* ActiveHealthBar;//这个变量是一个指向USWorldUserWidget类型的指针，表示当前正在显示的血量条小部件的实例，可以在代码中使用这个变量来更新血量条的显示或者销毁它。

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;//这个变量是一个类模板，表示一个用户界面小部件的类，可以在蓝图中设置这个变量来指定要使用哪个小部件来显示血量。

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;//这个变量是一个浮点数，表示在角色的材质上设置的参数，用来实现一些效果，比如闪红等。

	UPROPERTY(VisibleAnywhere)
	FName TargetActorKey;

	void SetTargetActor(AActor* NewTarget);//这个函数是用来设置目标角色的，可以在行为树中调用这个函数来更新AI角色的目标。

	AActor* GetTargetActor() const;//这个函数是用来设置目标角色的，可以在行为树中调用这个函数来更新AI角色的目标。

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	virtual void PostInitializeComponents() override;//这个函数是在组件初始化完成后调用的，可以在这里绑定事件或者进行其他初始化操作。

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;


	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
