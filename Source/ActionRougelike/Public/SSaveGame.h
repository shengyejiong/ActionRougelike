// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	/* Identifier for which Actor this belongs to */
	/* 译：识别这个数据属于哪个Actor */
	UPROPERTY()
	FString ActorName;

	/* For movable Actors, keep location,rotation,scale.*/
	/* 译：对于可移动的Actor，保存位置、旋转、缩放。 */
	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;

};

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API USSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;

};
