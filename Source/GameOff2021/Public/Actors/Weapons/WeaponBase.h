// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS(Abstract)
class GAMEOFF2021_API AWeaponBase : public AActor {
	GENERATED_BODY()

	bool bFiring;

	bool bReloading;
	
	FTimerHandle FireTimerHandle;

	FTimerHandle ReloadTimerHandle;

	void OnReloadFinished();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1"))
	float ReloadSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 AmmoMax;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 AmmoRemaining;
	
	int32 AmmoLoaded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSecondaryFireAvailable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bSecondaryFireAvailable", ClampMin = "0.0"))
	float FireRateSecondary;

	virtual void DoFire();

	virtual void DoStopFiring();

public:
	AWeaponBase();

	void Fire();

	void StopFiring();

	void Reload();

	bool CanFire() const;

	bool IsFiring() const { return bFiring; }
	
	bool SecondaryFireAvailable() const { return bSecondaryFireAvailable; }

	UFUNCTION(BlueprintCallable)
	float GetFireRate() const { return FireRate; }

	UFUNCTION(BlueprintCallable)
	int32 GetDamage() const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetAmmoLoaded() const { return AmmoLoaded; }

	UFUNCTION(BlueprintCallable)
	int32 GetAmmoRemaining() const { return AmmoRemaining; }

	int32 GetAmmoMax() const { return AmmoMax; }
};
