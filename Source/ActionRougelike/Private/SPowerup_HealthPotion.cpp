// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"


ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

}


void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	//这个函数是当玩家与这个物品交互时被调用的，我们需要在这里实现增加玩家生命值的逻辑。
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())//如果玩家的生命值组件存在，并且玩家的生命值没有满，我们就可以增加玩家的生命值。
	{

		if (AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax()))//我们调用玩家的生命值组件的ApplyHealthChange函数来增加玩家的生命值，增加的数值是玩家的最大生命值，这样就可以让玩家的生命值恢复到满。
		{
			HideAndCooldownPowerup();
		}
	}
}