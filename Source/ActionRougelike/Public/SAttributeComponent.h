// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);// 定义一个委托，参数分别是：造成伤害的Actor，属性组件本身，当前血量和血量变化值


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

protected:

	// EditAnywhere - 可以在蓝图编辑器和关卡编辑器中修改
	// VisibleAnywhere - 只能在蓝图编辑器和关卡编辑器中查看，不能修改（常用于组件和属性）
	// EditDefaultsOnly - 只能在蓝图编辑器中修改，不能在关卡编辑器中修改（常用于属性）
	// VisibleDefaultsOnly - 只能在蓝图编辑器中查看，不能修改(不常用）
	// EditInstanceOnly - 只能在关卡编辑器中修改，不能在蓝图编辑器中修改（只有被放置在关卡中的实例才有这个属性，常用于属性）
	// --
	// BlueprintReadOnly - 只能在蓝图中读取，不能修改（不影响‘details’面板中的属性）
	// BlueprintReadWrite - 可以在蓝图中读取和修改
	// -- 
	// Category - 在蓝图编辑器和关卡编辑器中显示的分类


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float HealthMax;


	// HeathMax, Stamina, Strength

public:	

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthMax() const;

	UPROPERTY(BlueprintAssignable)// 这个属性可以在蓝图中绑定事件
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);



		
};
