// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SWorldUserWidget.h"

//这个变量的意思是创建一个控制台变量，名字是su.InteractionDebugDraw，默认值是false，帮助信息是Enable Debug Lines for Interact Component，这个变量的作用是用来控制是否启用交互组件的调试线条功能的，如果这个变量的值是true，那么就会启用交互组件的调试线条功能了，这样就可以通过控制台命令来控制是否启用交互组件的调试线条功能了
static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component"), ECVF_Cheat);


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	CollisionChannel = ECC_WorldDynamic;
}




// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());//将拥有者转换为一个Pawn
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();//如果拥有者是本地控制的Pawn，那么就调用寻找最佳可交互对象的函数
	}
}


void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();//获取控制台变量的值，判断是否启用调试线条功能

	FCollisionObjectQueryParams ObjectQueryParams;//查询参数
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);//与物体交互，动态物体

	AActor* MyOwner = GetOwner();//获取拥有者

	FVector EyeLocation;//视点位置
	FRotator EyeRotation;//视点旋转

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);//获取视点位置和旋转

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);//视点位置加上视点旋转的向量乘以1000，得到一个远点位置

	//第一种碰撞检测方法
	//FHitResult Hit;//碰撞结果
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);//在世界中进行一条线性碰撞检测，起点为视点位置，终点为远点位置，使用查询参数，结果存储在Hit中，返回是否有碰撞


	//第二种碰撞检测方法
	TArray<FHitResult> Hits;//碰撞结果数组
	FCollisionShape Shape;//碰撞形状
	Shape.SetSphere(TraceRadius);//设置碰撞形状为一个半径为30的球体

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);//在世界中进行一个球形碰撞检测，起点为视点位置，终点为远点位置，使用查询参数，碰撞形状为一个半径为30的球体，结果存储在Hits数组中

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;//如果有碰撞，线的颜色为绿色，否则为红色

	FocusedActor = nullptr;//clear ref before trying to fill //在尝试填充之前清除引用

	for (FHitResult Hit : Hits)//遍历碰撞结果数组
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 0.0f);//在世界中绘制一个球体，位置为碰撞点，半径为30，分段数为32，颜色为LineColor，不持久化，持续2秒
		}
		AActor* HitActor = Hit.GetActor();
		if (HitActor)//如果碰撞结果中有一个演员
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;//将碰撞结果中的演员设置为焦点演员

				break;//如果有一个演员被交互了，跳出循环
			}
		}

	}

	if (FocusedActor)//如果玩家正在看着一个演员
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))//如果没有默认UI
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);//创建一个UI实例，使用世界作为上下文，使用默认的UI类

		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;//将UI实例的附加演员设置为焦点演员
			if (!DefaultWidgetInstance->IsInViewport())//如果UI实例不在视口中
			{
				DefaultWidgetInstance->AddToViewport();//将UI实例添加到视口中
			}
		}
	
	}
	else//如果玩家没有看着一个演员
	{
		
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();//将UI实例从父级中移除
		}
	}


	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 0.0f);//在世界中绘制一条线，起点为视点位置，终点为远点位置，颜色为LineColor，不持久化，持续2秒，线宽为2.0f
	}

}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);//调用服务器端的交互函数

}


void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{

	//如果没有焦点演员，输出一条调试消息，并返回
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());//将拥有者转换为一个Pawn
	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);//调用碰撞演员的交互函数，传入拥有者作为参数
}