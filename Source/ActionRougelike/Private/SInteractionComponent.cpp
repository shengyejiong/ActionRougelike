// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"

//这个变量的意思是创建一个控制台变量，名字是su.InteractionDebugDraw，默认值是false，帮助信息是Enable Debug Lines for Interact Component，这个变量的作用是用来控制是否启用交互组件的调试线条功能的，如果这个变量的值是true，那么就会启用交互组件的调试线条功能了，这样就可以通过控制台命令来控制是否启用交互组件的调试线条功能了
static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component"), ECVF_Cheat);


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void USInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();//获取控制台变量的值，判断是否启用调试线条功能

	FCollisionObjectQueryParams ObjectQueryParams;//查询参数
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);//与物体交互，动态物体

	AActor* MyOwner = GetOwner();//获取拥有者

	FVector EyeLocation;//视点位置
	FRotator EyeRotation;//视点旋转

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);//获取视点位置和旋转

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);//视点位置加上视点旋转的向量乘以1000，得到一个远点位置

	//第一种碰撞检测方法
	//FHitResult Hit;//碰撞结果
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);//在世界中进行一条线性碰撞检测，起点为视点位置，终点为远点位置，使用查询参数，结果存储在Hit中，返回是否有碰撞


	//第二种碰撞检测方法
	TArray<FHitResult> Hits;//碰撞结果数组

	float Radius = 30.0f;//碰撞半径

	FCollisionShape Shape;//碰撞形状
	Shape.SetSphere(Radius);//设置碰撞形状为一个半径为30的球体

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);//在世界中进行一个球形碰撞检测，起点为视点位置，终点为远点位置，使用查询参数，碰撞形状为一个半径为30的球体，结果存储在Hits数组中

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;//如果有碰撞，线的颜色为绿色，否则为红色

	for (FHitResult Hit : Hits)//遍历碰撞结果数组
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);//在世界中绘制一个球体，位置为碰撞点，半径为30，分段数为32，颜色为LineColor，不持久化，持续2秒
		}
		AActor* HitActor = Hit.GetActor();
		if (HitActor)//如果碰撞结果中有一个演员
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);//将拥有者转换为一个Pawn

				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);//调用碰撞演员的交互函数，传入拥有者作为参数
				break;//如果有一个演员被交互了，跳出循环
			}
		}

	}


	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);//在世界中绘制一条线，起点为视点位置，终点为远点位置，颜色为LineColor，不持久化，持续2秒，线宽为2.0f
	}



}
