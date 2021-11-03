// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosive.generated.h"

UCLASS(Abstract)
class GAMEOFF2021_API AExplosive : public AActor {
	GENERATED_BODY()

	/*
	 * Explosive -> Grenade, Mine, Bomb, Barrel
	 * Override for Gas/Effect function
	 * Interface/Component for Throwable
	 */
	
protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

public:
	AExplosive();
};
