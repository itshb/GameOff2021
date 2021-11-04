// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Explosives/Explosive.h"
#include "Grenade.generated.h"

/**
 * Basic HE Grenade
 */
UCLASS(Abstract)
class GAMEOFF2021_API AGrenade : public AExplosive {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Grenade")
	bool bDetonateOnImpact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Grenade", meta = (ClampMin = "0.1", EditCondition = "!bDetonateOnImpact"))
	float DetonationDelay;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	AGrenade();
};
