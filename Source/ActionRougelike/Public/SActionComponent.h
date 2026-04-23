// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameplayTagContainer.h>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"


class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// Tag是一种标识，可以用来标识一个对象的状态或者属性，比如一个角色可以有一个"Stunned"的Tag来表示它被眩晕了，一个物体可以有一个"Flammable"的Tag来表示它易燃了。GameplayTagContainer是一个容器类，可以存储多个Tag，并且提供了一些方法来操作这些Tag，比如添加、删除、查询等等。在这个组件中，ActiveGameplayTags就是用来存储当前激活的Tag的容器，这些Tag可以用来控制动作的执行或者其他逻辑。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);

	//这个函数是用来在蓝图中调用的，传入一个动作的实例，Instigator是执行这个动作的角色
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(USAction* ActionToRemove);

	//这个函数是用来在蓝图中调用的，传入一个动作的名字，Instigator是执行这个动作的角色
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	USAction* GetAction(TSubclassOf<USAction> ActionClass) const;

	// Sets default values for this component's properties
	USActionComponent();

protected:

	// 这个函数是用来在服务器上调用的，传入一个动作的名字，Instigator是执行这个动作的角色
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;

	UPROPERTY(Replicated)
	TArray<USAction*> Actions;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
