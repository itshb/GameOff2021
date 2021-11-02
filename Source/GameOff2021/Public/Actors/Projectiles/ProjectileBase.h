// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS(Abstract)
class GAMEOFF2021_API AProjectileBase : public AActor {
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* Collider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (ClampMin = "0"))
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	bool bDestroyOnOverlap;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnDestroy() {}
	
public:
	AProjectileBase();

	class USphereComponent* GetCollider() const { return Collider; }
	
	class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	int32 GetDamage() const { return Damage; }
};
