// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controllers/AIControllerBase.h"
#include "AI/Characters/AICharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/ProtagonistCharacter.h"

AAIControllerBase::AAIControllerBase() {
	BehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	ProtagonistKeyName = "Unset";
	DestinationKeyName = "Unset";
	AlertedKeyName = "Unset";
	AttackKeyName = "Unset";
}

void AAIControllerBase::OnPossess(class APawn* InPawn) {
	Super::OnPossess(InPawn);

	auto* AI = Cast<AAICharacterBase>(InPawn);
	if(AI) {
		if(ensure(AI->BehaviorTree->BlackboardAsset)) {
			Blackboard->InitializeBlackboard(*AI->BehaviorTree->BlackboardAsset);
		}

		BehaviorTree->StartTree(*AI->BehaviorTree);
	}
}

void AAIControllerBase::OnUnPossess() {
	Super::OnUnPossess();

	BehaviorTree->StopTree();
}

void AAIControllerBase::SetTargetEnemy(class AProtagonistCharacter* Target) {
	if(Blackboard) {
		Blackboard->SetValueAsObject(ProtagonistKeyName, Target);
	}
}

class AProtagonistCharacter* AAIControllerBase::GetTargetEnemy() const {
	return (Blackboard) ? Cast<AProtagonistCharacter>(Blackboard->GetValueAsObject(ProtagonistKeyName)) : nullptr;
}
