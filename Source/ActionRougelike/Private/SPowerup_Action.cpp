// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Action.h"
#include "SActionComponent.h"
#include "SAction.h"

void ASPowerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	// 确认我们有一个有效的InstigatorPawn和一个要授予的ActionToGrant
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	// 获取InstigatorPawn上的ActionComponent
	USActionComponent* ActionComp = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));

	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrant))
		{
			// 如果InstigatorPawn已经拥有这个Action了，我们就不需要再授予它了
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already know."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}

		ActionComp->AddAction(InstigatorPawn, ActionToGrant);
		HideAndCooldownPowerup();

	}

}

