// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UParticleSystem;
class UAudioComponent;
class USoundBase;
class UPrimitiveComponent;

UCLASS()
class ACTIONROUGELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USoundBase* ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float DestroyDelay = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	bool bDestroyed = true;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float ImpactShakeOuterRadius;

	UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// BlueprintNativeEvent = 表示这个函数既可以在C++中实现，也可以在蓝图中重写。如果在C++中没有提供实现，那么在蓝图中必须重写这个函数才能使用。
	// BlueprintCallable = 表示这个函数可以在蓝图中被调用。这使得设计师和程序员可以在蓝图中使用这个函数来实现游戏逻辑，而不需要编写C++代码。
	// 这个函数的作用是当投射物发生碰撞时触发爆炸效果。它可以在C++中实现默认的爆炸行为，也可以在蓝图中重写以实现自定义的爆炸效果。
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();


	// PostInitializeComponents()是AActor类中的一个虚函数，在Actor的组件初始化完成后被调用。这个函数通常用于在组件初始化完成后执行一些额外的设置或逻辑，例如绑定事件、设置初始状态等。
	virtual void PostInitializeComponents() override;


public:	
	
	ASProjectileBase();

};



