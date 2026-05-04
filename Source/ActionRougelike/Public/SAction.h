// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"


class UWorld;
class USActionComponent;


USTRUCT()
struct FActionRepData //这个结构体是用来在网络中同步动作的状态的，包含了一个布尔值表示动作是否正在运行，以及一个指向执行这个动作的角色的指针
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

/**
 * 
 */
UCLASS(Blueprintable)//uclass是用来声明一个类的，Blueprintable是用来告诉UE这个类可以被蓝图继承的
class ACTIONROUGELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;//这个属性是用来在UI中显示这个动作的图标的，TSoftObjectPtr是一个软对象指针，可以用来存储一个对象的引用，这样就可以在需要的时候加载这个对象，而不是在游戏开始时就加载所有的对象，这样可以节省内存和加载时间

	UPROPERTY(Replicated)//这个属性是用来在运行时存储这个动作所属的组件的，这样我们就可以通过这个组件来访问角色的Tag或者其他属性
	USActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;//这个函数是用来获取这个动作所属的组件的，这样我们就可以通过这个组件来访问角色的Tag或者其他属性

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;//这个属性是用来在动作执行时给角色添加一些Tag的，比如一个攻击动作可以给角色添加一个"Attacking"的Tag，这样其他系统就可以根据这个Tag来判断角色是否在攻击状态

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;//这个属性是用来在动作执行时检查角色是否有一些Tag的，如果有的话就不能执行这个动作，比如一个攻击动作可以检查角色是否有一个"Stunned"的Tag，如果有的话就不能执行这个攻击动作

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")//replicatedusing是用来告诉UE这个属性需要在网络中同步，并且当这个属性发生变化时需要调用一个函数来处理这个变化
	FActionRepData RepData;
	//bool bIsRunning;//记录动作是否正在进行

	UPROPERTY(Replicated)
	float TimeStarted;

	UFUNCTION()
	void OnRep_RepData();

public:

	void Initialize(USActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;//这个属性是用来设置这个动作是否在角色生成时自动开始的，如果设置为true，那么当角色生成时这个动作就会自动开始执行

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;//这个函数是用来检查这个动作是否正在运行的，返回一个布尔值表示是否正在运行


	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator) const;//这个函数是用来检查这个动作是否可以被执行的，传入一个角色作为参数，返回一个布尔值表示是否可以执行这个动作

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UWorld* GetWorld() const override;//重写GetWorld函数，使得这个对象能够访问到世界对象

	bool IsSupportedForNetworking() const override
	{
		return true;//允许网络同步
	}
};


