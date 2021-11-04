// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFired, int32, NewAmmoLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoUpdated, int32, NewAmmoLoaded, int32, NewAmmoRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReloadStarted, float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadCancelled);

UCLASS(Abstract)
class GAMEOFF2021_API AWeapon : public AActor {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MuzzleLocation;

	bool bFiring;

	bool bReloading;
	
	FTimerHandle FireTimerHandle;

	FTimerHandle ReloadTimerHandle;

	void OnReloadFinished();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (ClampMin = "0.0"))
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (ClampMin = "0.1"))
	float ReloadSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (ClampMin = "1"))
	int32 AmmoMax;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (ClampMin = "0"))
	int32 AmmoRemaining;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (ClampMin = "1"))
	int32 MagazineSize;

	int32 AmmoLoaded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool bSecondaryFireAvailable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (EditCondition = "bSecondaryFireAvailable", ClampMin = "0.0"))
	float FireRateSecondary;

	virtual void DoFire();

public:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponFired OnWeaponFired;

	UPROPERTY(BlueprintAssignable)
	FOnAmmoUpdated OnAmmoUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnReloadStarted OnReloadStarted;

	UPROPERTY(BlueprintAssignable)
	FOnReloadCancelled OnReloadCancelled;

	AWeapon();

	void Fire();

	void StopFiring();

	void Reload();

	void StopReloading();

	bool CanFire() const;

	bool IsFiring() const { return bFiring; }

	bool IsReloading() const { return bReloading; }

	bool CanReload() const;
	
	bool SecondaryFireAvailable() const { return bSecondaryFireAvailable; }

	void AddAmmo(const int32 Value);

	UFUNCTION(BlueprintCallable)
	float GetFireRate() const { return FireRate; }

	UFUNCTION(BlueprintCallable)
	int32 GetDamage() const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetAmmoLoaded() const { return AmmoLoaded; }

	UFUNCTION(BlueprintCallable)
	int32 GetAmmoRemaining() const { return AmmoRemaining; }

	int32 GetAmmoMax() const { return AmmoMax; }

	class USkeletalMeshComponent* GetMesh() const { return Mesh; }

	FVector GetMuzzleLocation() const { return MuzzleLocation->GetComponentLocation(); }
	
	void AttachTo(class AActor* ParentActor, const FName& SocketName);
};
