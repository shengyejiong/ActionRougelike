// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"



ASAICharacter::ASAICharacter()
{

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}



void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);//这个函数是一个事件绑定函数，当PawnSensingComp组件感知到一个Pawn时，会调用OnPawnSeen函数。
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);//给属性组件添加监听，当属性发生变化时会调用OnHealthChanged函数
}



void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{

		if (InstigatorActor != this)//这个条件是用来判断造成伤害的InstigatorActor是否是当前角色本身，如果不是当前角色本身，说明是其他角色造成的伤害，可以进行后续操作，比如播放受伤动画、显示血量等。
		{
			SetTargetActor(InstigatorActor);//这个函数是用来设置目标角色的，可以在行为树中调用这个函数来更新AI角色的目标。这里将造成伤害的InstigatorActor设置为目标角色，这样AI角色就会知道是谁造成了伤害，可以进行相应的反应，比如攻击回去或者逃跑等。
		}


		if (NewHealth <= 0.0f)
		{
			//停止BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed"); //大脑组件其实就是行为树组件
			}

			//ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);//这个函数是将角色的所有骨骼设置为模拟物理，这样角色就会变成一个布娃娃，受到物理引擎的影响，可以被推开、倒地等。
			GetMesh()->SetCollisionProfileName("Ragdoll");//这个函数是将角色的碰撞配置文件设置为"Ragdoll"，这样角色就会使用布娃娃的碰撞设置，可以与其他物体发生碰撞并产生物理反应。


			//设置生命周期
			SetLifeSpan(10.0f);
		}
	}
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());//这个函数是将当前角色的控制器转换为AAIController类型，如果转换成功，说明当前角色是由AI控制的，可以进行后续操作。
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);//这个函数是将目标角色设置到黑板组件中，键名为"TargetActor"，值为NewTarget，这样行为树中的任务就可以从黑板中获取目标角色的信息来执行相应的行为。

	}
}


void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);//当PawnSensingComp组件感知到一个Pawn时，会调用OnPawnSeen函数，在这个函数中会调用SetTargetActor函数将感知到的Pawn设置为目标角色，这样行为树中的任务就可以从黑板中获取目标角色的信息来执行相应的行为。

	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);//这个函数是绘制一个调试字符串在游戏世界中，位置为当前角色的位置，文本内容为"PLAYER SPOTTED"，颜色为白色，持续时间为4秒，是否在屏幕上显示为true。这个函数可以用来调试和可视化AI角色的感知状态。
	
}


