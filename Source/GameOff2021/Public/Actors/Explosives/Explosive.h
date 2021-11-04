// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosive.generated.h"

UCLASS()
class GAMEOFF2021_API AExplosive : public AActor {
	GENERATED_BODY()

	/*
	 * Explosive -> Grenade, Mine, Bomb, Barrel
	 * Override for Gas/Effect function
	 * Interface/Component for Throwable
	 */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh; // Static until need of Skeletal

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ExplosionCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* ExplosionVFX;

	class UNiagaraComponent* VFXActor;

	bool bExploding;

	float Elapsed;

	float Progress;

	void DoExplode();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = "0"))
	int32 Damage;

	/** 
	 * How long does the explosion last & deal damage
	 * zero for instant destruction only
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = "0.0"))
	float ExplosionLifeTime;
	
	virtual void Tick(float DeltaTime) override;

	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	AExplosive();

	virtual void Explode();

	class UStaticMeshComponent* GetMesh() const { return Mesh; }

	int32 GetDamage() const { return Damage; }

	float GetRadius() const;

	bool IsExploding() const { return bExploding; }

	float GetLifeTime() const { return ExplosionLifeTime; }

	float GetTimeLeft() const { return Progress; }

	class UNiagaraSystem* GetExplosionVFX() const { return ExplosionVFX; }
};
