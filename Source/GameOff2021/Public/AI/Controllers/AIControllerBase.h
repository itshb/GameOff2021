// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class GAMEOFF2021_API AAIControllerBase : public AAIController {
	GENERATED_BODY()

	class UBehaviorTreeComponent* BehaviorTree;

	class UBlackboardComponent* Blackboard;

protected:
	virtual void OnPossess(class APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName ProtagonistKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName DestinationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName AlertedKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName AttackKeyName;

public:
	AAIControllerBase();

	void SetTargetEnemy(class AProtagonistCharacter* Target);
	
	class UBehaviorTreeComponent* GetBehaviorTree() const { return BehaviorTree; }

	class UBlackboardComponent* GetBlackboard() const { return Blackboard; }

	class AProtagonistCharacter* GetTargetEnemy() const;
};
