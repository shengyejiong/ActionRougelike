// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "SWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SActionComponent.h"



ASAICharacter::ASAICharacter()
{

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);//这个函数是用来设置角色的胶囊组件对世界动态物体的碰撞响应为忽略，这样角色就不会与世界动态物体发生碰撞了，可以避免一些不必要的碰撞检测和物理反应，提高性能。
	GetMesh()->SetGenerateOverlapEvents(true);//这个函数是用来设置角色的网格组件生成重叠事件的，这样当角色与其他物体发生重叠时，就会触发重叠事件，可以在代码中监听这些事件来实现一些功能，比如感知、攻击等。

	TimeToHitParamName = "TimeToHit";//这个变量是一个浮点数，表示在角色的材质上设置的参数
	TargetActorKey = "TargetActor";
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


		if (ActiveHealthBar == nullptr)//这个条件是用来判断当前是否已经有一个血量条小部件在显示，如果没有，就创建一个新的血量条小部件并添加到视口中，这样就可以在游戏中看到血量条了。
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);//这个函数是用来创建一个新的小部件实例的，参数是当前的世界和要使用的小部件类，这里创建了一个USWorldUserWidget类型的小部件实例，并将其赋值给ActiveHealthBar变量，这样就可以在代码中使用ActiveHealthBar来更新血量条的显示或者销毁它。
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;//这个函数是用来将小部件绑定到一个角色上的，这样小部件就会跟随角色移动，并且可以在小部件中获取角色的信息来更新显示。这里将ActiveHealthBar绑定到当前角色上，这样血量条的小部件就会跟随角色移动，并且可以在小部件中获取角色的信息来更新显示。
				ActiveHealthBar->AddToViewport();//这个函数是用来将小部件添加到视口中的，这样就可以在游戏中看到这个小部件了。这里将ActiveHealthBar添加到视口中，这样就会显示血量条的小部件。
			}
		}	

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);//如果血量减少了，就在角色的材质上设置一个参数，这个参数的值是当前的世界时间，这样就可以在材质中根据这个参数来实现一些效果，比如闪红等

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

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);//角色死亡后，禁用胶囊组件的碰撞，这样角色就不会与其他物体发生碰撞了，可以避免一些不必要的碰撞检测和物理反应，提高性能。
			GetCharacterMovement()->DisableMovement();//这个函数是用来禁用角色的移动组件的，这样角色就无法再移动了，可以避免一些不必要的移动计算和输入处理，提高性能。

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
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);//这个函数是从AI控制器的黑板组件中设置一个对象类型的值，这个值的键是TargetActorKey，这个键是在ASAICharacter类中定义的一个字符串变量，表示目标角色的键。这个函数会将NewTarget这个AActor类型的指针设置为目标角色，这样行为树中的任务就可以从黑板中获取目标角色的信息来执行相应的行为。
	}
}

AActor* ASAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());//这个函数是将当前角色的控制器转换为AAIController类型，如果转换成功，说明当前角色是由AI控制的，可以进行后续操作。
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));//这个函数是从AI控制器的黑板组件中获取一个对象类型的值，这个值的键是TargetActorKey，这个键是在ASAICharacter类中定义的一个字符串变量，表示目标角色的键。这个函数会返回一个AActor类型的指针，如果成功获取到目标角色，就会返回这个指针，否则返回nullptr。
	}

	return nullptr;
}


void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	// 如果目标已经设定，就不需要再设定了
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);//当PawnSensingComp组件感知到一个Pawn时，会调用OnPawnSeen函数，在这个函数中会调用SetTargetActor函数将感知到的Pawn设置为目标角色，这样行为树中的任务就可以从黑板中获取目标角色的信息来执行相应的行为。

		USWorldUserWidget* NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (NewWidget)
		{
			NewWidget->AttachedActor = this;//这个函数是用来将小部件绑定到一个角色上的，这样小部件就会跟随角色移动，并且可以在小部件中获取角色的信息来更新显示。这里将NewWidget绑定到当前角色上，这样当AI角色感知到玩家时，"PLAYER SPOTTED"的小部件就会跟随AI角色移动，并且可以在小部件中获取角色的信息来更新显示。

			NewWidget->AddToViewport(10);//这个函数是用来将小部件添加到视口中的，这样就可以在游戏中看到这个小部件了。这里将NewWidget添加到视口中，并且设置ZOrder为10，这样就会显示"PLAYER SPOTTED"的小部件，并且它会在其他UI元素的上方显示。
		}

	}




	//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);//这个函数是绘制一个调试字符串在游戏世界中，位置为当前角色的位置，文本内容为"PLAYER SPOTTED"，颜色为白色，持续时间为4秒，是否在屏幕上显示为true。这个函数可以用来调试和可视化AI角色的感知状态。
}


