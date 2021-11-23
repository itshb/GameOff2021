// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThrowComponent.generated.h"

/**
 * Component used for throwing other actors, E.g Grenades & Rocks
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAMEOFF2021_API UThrowComponent final : public UActorComponent {
	GENERATED_BODY()

	class USplineComponent* Spline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Throw, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* LaunchPoint;

	TArray<class USplineMeshComponent*> Meshes;

	FVector Velocity() const;

	void CalculateTrajectory();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Throw, meta = (ClampMin = "1.0", ClampMax = "2.0"))
	float ForceMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Throw)
	class UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Throw)
	class UMaterialInterface* Material;

	UThrowComponent();

	void DisplayTrajectory(bool bState);

	void UpdateForceMultiplier(const float Value);

	void ThrowGrenade(TSubclassOf<class AGrenade> Grenade);
};
