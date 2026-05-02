// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASPowerup_HealthPotion : public ASPowerupActor
{
	GENERATED_BODY()
	

public:

	ASPowerup_HealthPotion();

	void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	int CreditCost;

protected:



};
