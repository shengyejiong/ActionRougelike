// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"
#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(MyPawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(MyPawn, AttributeComp->GetHealthMax());
	}

	return EBTNodeResult::Succeeded;
}