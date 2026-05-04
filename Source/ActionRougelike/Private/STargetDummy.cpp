// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);//给根组件添加监听，当属性发生变化时会调用OnHealthChanged函数


}


void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);//这个函数是用来设置材质参数的，当属性发生变化时会调用这个函数，设置材质参数TimeToHit为当前时间，这样就可以在材质中使用这个参数来实现一些效果，比如闪烁或者变色等
	}

}

