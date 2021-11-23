// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacterBase.generated.h"

UCLASS(Abstract)
class GAMEOFF2021_API AAICharacterBase : public ACharacter {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;
		
	float LastSeenTime;

	float LastHeardTime;

	float SenseTimeout;

	bool bSensedTarget;

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnSeePlayer(class APawn* Pawn);
	
	UFUNCTION()
	void OnHearNoise(class APawn* PawnInstigator, const FVector& Location, float Volume);

	void UseWeapon();

public:
	AAICharacterBase();
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;
};
