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
	bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition);//这个函数是用来将世界坐标转换为屏幕坐标的，它接受三个参数：第一个参数是获取当前玩家控制器的函数，第二个参数是要转换的世界坐标，这里是绑定的Actor的位置加上一个偏移量，第三个参数是一个引用参数，用来存储转换后的屏幕坐标。这个函数会返回一个布尔值，表示转换是否成功，如果成功了，那么屏幕坐标就会被存储在ScreenPosition变量中。

	if (bIsOnScreen)//将世界坐标转换为屏幕坐标
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);//获取视口缩放比例

		ScreenPosition /= Scale;//根据缩放比例调整屏幕位置

		if (ParentSizeBox)//如果绑定了SizeBox组件
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);//设置SizeBox组件的渲染平移，使其在屏幕上正确显示

		}
	}

	if (ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);//根据是否在屏幕上设置SizeBox组件的可见性，如果在屏幕上就设置为HitTestInvisible，否则设置为Collapsed，这样就可以根据Actor是否在屏幕上来显示或者隐藏血量条的小部件了。
	}
}
