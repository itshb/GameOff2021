// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS(Abstract)
class GAMEOFF2021_API AWeaponBase : public AActor {
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
	TSubclassOf<class AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool bSecondaryFireAvailable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (EditCondition = "bSecondaryFireAvailable", ClampMin = "0.0"))
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

	bool CanReload() const;
	
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

	class USkeletalMeshComponent* GetMesh() const { return Mesh; }

	FVector GetMuzzleLocation() const { return MuzzleLocation->GetComponentLocation(); }
	
	void AttachTo(class AActor* ParentActor, const FName& SocketName);
};
