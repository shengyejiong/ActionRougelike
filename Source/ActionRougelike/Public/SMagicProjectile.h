// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SProjectileBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SMagicProjectile.generated.h"


class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

class USActionEffect;

UCLASS()
class ACTIONROUGELIKE_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:
	bool bHasAppliedHit = false;

	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;//创建一个ParryTag标签，用于攻击格挡时的标签判断

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<USActionEffect> BurningActionClass;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UProjectileMovementComponent* MovementComp;


};
