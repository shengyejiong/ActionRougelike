// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


USBTService_CheckHealth::USBTService_CheckHealth()//这个函数是构造函数，在创建对象时会被调用
{
	LowHealthFraction = 0.3f;
}

//这个函数是TickNode函数，在每一帧都会被调用，可以在这里实现检查血量的逻辑
void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(AIPawn);
		if (ensure(AttributeComp))
		{
			bool bLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetHealthMax()) < LowHealthFraction;

			UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
			BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}