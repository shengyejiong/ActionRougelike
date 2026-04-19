// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void PrimaryInteract();

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:

	// Reliable - 将会永远尝试发送这个RPC，直到成功为止。适用于那些必须成功执行的操作，比如玩家输入、重要事件等。
	// Unreliable - 不保证RPC一定会被发送或执行，可能会丢失。适用于那些不太重要的操作，比如动画更新、非关键状态同步等。
	// Server - 这个RPC只能由客户端调用，并且会在服务器上执行。适用于那些需要服务器验证或处理的操作，比如玩家交互、物品拾取等。

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);//服务器端的交互函数,参数是当前聚焦的对象

	void FindBestInteractable();//寻找最佳的可交互对象

	virtual void BeginPlay() override;

	UPROPERTY()
	AActor* FocusedActor;//当前聚焦的对象

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;//碰撞检测的距离

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;//碰撞检测的半径

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;//碰撞通道
	//这里用TEnumAsByte是因为ECollisionChannel是一个枚举类型，而C++枚举大小不确定，无法被UE的反射系统识别，所以需要用TEnumAsByte来包装一下，告诉UE这个枚举类型的大小是一个字节，这样就可以被UE的反射系统识别了

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;//默认的UI类

	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;//默认的UI实例

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
