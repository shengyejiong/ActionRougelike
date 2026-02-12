// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"



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
	FCollisionObjectQueryParams ObjectQueryParams;//查询参数
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);//与物体交互，动态物体

	AActor* MyOwner = GetOwner();//获取拥有者

	FVector EyeLocation;//视点位置
	FRotator EyeRotation;//视点旋转

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);//获取视点位置和旋转

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);//视点位置加上视点旋转的向量乘以1000，得到一个远点位置

	FHitResult Hit;//碰撞结果
	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);//从视点位置到远点位置进行线性碰撞检测，检测对象类型为动态物体

	AActor* HitActor = Hit.GetActor();
	if (HitActor)//如果碰撞结果中有一个演员
	{
		if (HitActor->Implements<USGameplayInterface>())
		{
			APawn* MyPawn = Cast<APawn>(MyOwner);//将拥有者转换为一个Pawn

			ISGameplayInterface::Execute_Interact(HitActor, MyPawn);//调用碰撞演员的交互函数，传入拥有者作为参数
		}
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Red, false, 2.0f, 0, 2.0f);//在世界中绘制一条从视点位置到远点位置的红色线，持续2秒，线宽为2.0f

}
