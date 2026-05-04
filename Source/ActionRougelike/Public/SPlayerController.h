// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

// 声明一个委托，当玩家控制的Pawn发生变化时触发
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	void SetupInputComponent() override;

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateReceived;

	virtual void SetPawn(APawn* InPawn) override;

	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();

	void OnRep_PlayerState() override;
};
