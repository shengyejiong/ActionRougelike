// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"
#include "Misc/AssertionMacros.h"
#include "Math/RotationMatrix.h"          // 用于 FRotationMatrix 和 MakeFromX


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");//交互组件的实现

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);//交互输入绑定

	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);

}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//X:前向（红色）
	//Y:右向（绿色）
	//Z:上向（蓝色）

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);//播放攻击动画

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);//设置一个定时器，在0.2秒后调用PrimaryAttack_TimeElapsed函数

}


void ASCharacter::PrimaryAttack_TimeElapsed()//这个函数在攻击动画的0.2秒时被调用，可以在这里添加攻击逻辑，比如造成伤害、播放特效等
{
	SpawnProjectile(ProjectileClass);//生成投射物
}


void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);//播放攻击动画

	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeAttack, this, &ASCharacter::BlackholeAttack_TimeElapsed, AttackAnimDelay);//设置一个定时器，在0.2秒后调用BlackholeAttack_TimeElapsed函数

}

void ASCharacter::BlackholeAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);//生成黑洞投射物
}

void ASCharacter::Dash()
{
	PlayAnimMontage(AttackAnim);//播放攻击动画

	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, AttackAnimDelay);//设置一个定时器，在0.2秒后调用Dash_TimerElapsed函数
}

void ASCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);//生成冲刺投射物
}



void ASCharacter::SpawnProjectile(TSubclassOf <AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))//检查要生成的类是否有效，如果无效则输出错误信息并中断程序，但在发布版本中会被忽略，所以可以安全地使用来检查类是否有效
	{
		FVector HandleLocation = GetMesh()->GetSocketLocation("Muzzle_01");//获取发射点位置

		FActorSpawnParameters SpawnParams;//生成参数
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//忽略碰撞直接生成
		SpawnParams.Instigator = this;//设置生成者为自己

		FCollisionShape Shape;//碰撞形状
		Shape.SetSphere(20.0f);//设置为一个半径为20的球形


		FCollisionQueryParams Params;//碰撞查询参数
		Params.AddIgnoredActor(this);//忽略自己

		FCollisionObjectQueryParams ObjParams;//碰撞对象查询参数
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);//查询动态物体
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);//查询静态物体
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);//查询角色

		FVector TraceStart = CameraComp->GetComponentLocation();//从摄像机位置开始

		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);//向摄像机朝向的方向发出一条长度为5000的射线

		FHitResult Hit;//碰撞结果

		if(GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))//如果射线与某个物体发生了碰撞
		{
			TraceEnd = Hit.ImpactPoint;//将射线的终点设置为碰撞点，这样生成的投射物就会朝向碰撞点飞行
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandleLocation).Rotator();//根据发射点位置和朝向计算生成的投射物的旋转

		FTransform SpawnTM = FTransform(ProjRotation, HandleLocation);//生成的投射物的变换
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);//生成投射物
	}

}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)//如果交互组件存在
	{
		InteractionComp->PrimaryInteract();//调用交互组件的交互函数
	}
	
}