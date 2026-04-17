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
