// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThrowComponent.generated.h"

/**
 * Component used for throwing other actors, E.g Grenades & Rocks
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEOFF2021_API UThrowComponent final : public UActorComponent {
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector Velocity() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Throw, meta = (ClampMin = "1.0", ClampMax = "2.0"))
	float ForceMultiplier;

	UThrowComponent();

	void DisplayTrajectory(bool bState);

	void ThrowGrenade(TSubclassOf<class AGrenade> Grenade);
};
