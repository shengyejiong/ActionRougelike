// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameplayTagContainer.h>
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"


class UWorld;

/**
 * 
 */
UCLASS(Blueprintable)//uclass是用来声明一个类的，Blueprintable是用来告诉UE这个类可以被蓝图继承的
class ACTIONROUGELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;//这个函数是用来获取这个动作所属的组件的，这样我们就可以通过这个组件来访问角色的Tag或者其他属性

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;//这个属性是用来在动作执行时给角色添加一些Tag的，比如一个攻击动作可以给角色添加一个"Attacking"的Tag，这样其他系统就可以根据这个Tag来判断角色是否在攻击状态

	UPROPERTY(EditDefaultsonly, Category = "Tags")
	FGameplayTagContainer BlockedTags;//这个属性是用来在动作执行时检查角色是否有一些Tag的，如果有的话就不能执行这个动作，比如一个攻击动作可以检查角色是否有一个"Stunned"的Tag，如果有的话就不能执行这个攻击动作

	bool bIsRunning;//记录动作是否正在进行

public:

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;//这个函数是用来检查这个动作是否正在运行的，返回一个布尔值表示是否正在运行

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator) const;//这个函数是用来检查这个动作是否可以被执行的，传入一个角色作为参数，返回一个布尔值表示是否可以执行这个动作

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UWorld* GetWorld() const override;//重写GetWorld函数，使得这个对象能够访问到世界对象
};
