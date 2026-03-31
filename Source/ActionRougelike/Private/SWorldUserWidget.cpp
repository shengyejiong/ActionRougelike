// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))//如果绑定的Actor无效或者已经被销毁，就直接返回，不进行后续的更新操作。
	{
		RemoveFromParent();//这个函数是用来将小部件从父级小部件中移除的，这样就不会再显示这个小部件了。这里调用RemoveFromParent函数来移除当前的小部件，这样当绑定的Actor无效或者已经被销毁时，血量条的小部件就会被移除，不再显示在游戏中。

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing widget from Health Widget."));
		return;
	}

	FVector2D ScreenPosition;//屏幕位置

	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))//将世界坐标转换为屏幕坐标
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);//获取视口缩放比例

		ScreenPosition /= Scale;//根据缩放比例调整屏幕位置

		if (ParentSizeBox)//如果绑定了SizeBox组件
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);//设置SizeBox组件的渲染平移，使其在屏幕上正确显示

		}
	}
}
