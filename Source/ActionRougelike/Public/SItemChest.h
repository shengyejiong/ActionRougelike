// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROUGELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	void Interact_Implementation(APawn* InstigatorPawn);

	void OnActorLoaded_Implementation();
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:

	// ReplicatedUsing的意思是当这个变量在服务器上发生变化的时候，客户端会调用这个函数来更新状态，这样就可以保证服务器和客户端之间的状态是一致的
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadOnly, SaveGame)
	bool bLidOpened;//这个变量的意思是一个布尔类型的变量，名字是bLidOpened，表示箱子的盖子是否打开了，这个变量被标记为Replicated，说明它是一个需要在网络上进行复制的变量，也就是说当这个变量在服务器上发生变化的时候，客户端也会相应地发生变化了，这样就可以保证服务器和客户端之间的状态是一致的了

	UFUNCTION()
	void OnRep_LidOpened();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;



};
