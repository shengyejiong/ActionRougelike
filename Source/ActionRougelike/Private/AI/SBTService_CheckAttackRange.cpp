
#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

/*
	这个函数是BTService_CheckAttackRange类的TickNode函数的实现。BTService_CheckAttackRange是一个行为树服务节点，用于检查AI是否在攻击范围内。
	TickNode函数是BTService类中的一个虚函数，用于在行为树的每个tick（更新周期）中执行特定的逻辑。在这个函数中，你可以编写代码来检查AI与目标之间的距离，判断是否在攻击范围内，并根据结果更新黑板中的相关变量，以便行为树的其他节点可以使用这些信息来做出决策。
*/
void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);// 调用父类的TickNode函数，确保基础功能正常执行

	// 检查ai角色和目标之间的距离是否在攻击范围内

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();// 获取黑板组件，用于访问和修改黑板中的变量
	if (ensure(BlackBoardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));// 从黑板中获取目标Actor对象
		if (TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();// 获取AI控制器
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();// 获取AI控制的Pawn对象
				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());// 计算目标Actor和AI Pawn之间的距离

					bool bWithinRange = DistanceTo < 2000.f;// 判断距离是否在攻击范围内，这里假设攻击范围是2000单位

					bool bHasOS = false;// 初始化一个布尔变量，用于存储AI是否能够看到目标Actor的结果
					if (bWithinRange)
					{
						bHasOS = MyController->LineOfSightTo(TargetActor);// 检查AI是否能够看到目标Actor，这个函数会更新AI控制器的感知系统，确保AI知道目标的位置和状态
					}
					
					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasOS));// 是否在攻击范围内，并且能看到目标Actor，将结果存储在黑板中，以便行为树的其他节点可以使用这个信息来做出决策
				}
			}
		}
	}
}
