// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

// 这个define是用来定义本地化文本的命名空间的，我们在GetInteractText_Implementation函数中使用了LOCTEXT宏来定义本地化文本，这个宏需要一个命名空间参数来区分不同的文本资源，所以我们需要定义一个命名空间来使用这个宏。
#define LOCTEXT_NAMESPACE "InteractableActors"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	CreditCost = 50;
}


void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	//这个函数是当玩家与这个物品交互时被调用的，我们需要在这里实现增加玩家生命值的逻辑。
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);//我们需要先获取玩家的生命值组件，因为我们需要在增加玩家生命值时调用这个组件的函数来增加玩家的生命值。

	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())//如果玩家的生命值组件存在，并且玩家的生命值没有满，我们就可以增加玩家的生命值。
	{
		//我们需要先获取玩家的PlayerState，因为我们可能需要在增加玩家生命值后更新玩家的UI界面，显示玩家当前的生命值。
		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			//我们需要先检查玩家是否有足够的金币来购买这个生命药水，如果玩家没有足够的金币，我们就不能增加玩家的生命值。
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerup();
			}
				
		}

		
	}
}

FText ASPowerup_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Alreary at full health");//如果玩家的生命值已经满了，我们就返回一个提示文本，告诉玩家他们已经满血了。
	}

	return FText::Format(LOCTEXT("HealthPotion_InteractText", "Cost {0} Credits. Restores health to maximun"), CreditCost);
}


#undef LOCTEXT_NAMESPACE