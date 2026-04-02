// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;

UCLASS()
class ACTIONROUGELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;//绑定一个SizeBox组件，用于调整UI元素的大小

	/*
	* 这个函数是UUserWidget的一个虚函数，用于在每一帧更新用户界面。它接受两个参数：MyGeometry表示当前用户界面的几何信息，InDeltaTime表示自上次更新以来的时间增量。通过重写这个函数，你可以在每一帧执行自定义的逻辑，例如更新UI元素的位置、处理动画或者响应用户输入等。
	*/
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


public:

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;//这个变量是一个三维向量，表示UI元素在世界空间中的偏移量，可以用来调整UI元素的位置，使其更好地适应游戏场景。

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachedActor;
};
