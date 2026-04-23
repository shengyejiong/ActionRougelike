// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"



// Sets default values
ASItemChest::ASItemChest()
{

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

	SetReplicates(true);//启用复制，这样这个Actor就可以在网络上被复制了，也就是说当这个Actor在服务器上被创建或者销毁的时候，客户端也会相应地创建或者销毁这个Actor了，这样就可以保证服务器和客户端之间的状态是一致的了
}



void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;//当这个函数被调用的时候，就会将bLidOpened的值取反了，也就是说如果bLidOpened原来是false，那么就会变成true了，如果bLidOpened原来是true，那么就会变成false了，这样就可以通过调用这个函数来控制bLidOpened的值了，这个函数的作用就是当玩家与这个箱子进行交互的时候，就会打开或者关闭箱子的盖子
	OnRep_LidOpened();//用来更新状态

}

void ASItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;//如果bLidOpened是true，那么就将CurrPitch设置为TargetPitch了，也就是说当箱子被打开的时候，CurrPitch就会等于TargetPitch了，如果bLidOpened是false，那么就将CurrPitch设置为0.0f了，也就是说当箱子被关闭的时候，CurrPitch就会等于0.0f
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));//将盖子的相对旋转设置为一个新的旋转，这个旋转的Pitch值就是CurrPitch，也就是说当箱子被打开的时候，盖子的Pitch值就会等于TargetPitch，当箱子被关闭的时候，盖子的Pitch值就会等于0.0f
}


// 这个函数的意思是重写了父类的GetLifetimeReplicatedProps函数，这个函数是用来告诉UE哪些变量需要被复制的，在这个函数中调用了父类的GetLifetimeReplicatedProps函数，然后使用了一个宏DOREPLIFETIME来告诉UE这个变量需要被复制了，也就是说当这个变量在服务器上发生变化的时候，客户端也会相应地发生变化
void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME(ASItemChest, bLidOpened);//这个宏的意思是告诉UE这个变量需要被复制了，也就是说当这个变量在服务器上发生变化的时候，客户端也会相应地发生变化
}