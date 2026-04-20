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

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);// 定义一个静态函数，用于从一个Actor中获取属性组件，如果该Actor没有属性组件，则返回nullptr

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);// 定义一个静态函数，用于判断一个Actor是否存活，如果该Actor没有属性组件或者血量小于等于0，则返回false；否则返回true

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


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float HealthMax;

	// HeathMax, Stamina, Strength

	//UPROPERTY(ReplicatedUsing="")
	//bool bIsAlive;

	// Replicated - 这个属性会在网络上进行复制，也就是说当这个属性在服务器上发生变化的时候，客户端也会相应地发生变化，这样就可以保证服务器和客户端之间的状态是一致的
	// @FIXME: mark as unreliable once we moved the 'state' out of scharacter - 修复：当我们把状态从角色中移出来之后，标记为不可靠，因为我们不需要保证状态的绝对一致性了，只需要保证大致的一致性，这样可以减少网络带宽的使用和提高性能
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* Instigator, float NewHealth, float Delta);

public:	

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthMax() const;

	UPROPERTY(BlueprintAssignable)// 这个属性可以在蓝图中绑定事件
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	float GetHealth();

		
};
