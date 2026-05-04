// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API USBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()
	
	

public:
	USBTService_CheckHealth();

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float LowHealthFraction;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector LowHealthKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
