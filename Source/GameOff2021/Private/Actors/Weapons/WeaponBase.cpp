// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/WeaponBase.h"
#include "Actors/Projectiles/ProjectileBase.h"

AWeaponBase::AWeaponBase() {
	PrimaryActorTick.bCanEverTick = false;
}

void AWeaponBase::Fire() {
	if(!CanFire()) return;
	bFiring = true;

	if(FireRate == 0) {
		DoFire();
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AWeaponBase::DoFire, FireRate, true);
}

void AWeaponBase::StopFiring() {
	if(!bFiring) return;
	bFiring = false;

	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void AWeaponBase::Reload() {
	if(bReloading) return;
	bReloading = true;

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::OnReloadFinished, ReloadSpeed, false);
}

void AWeaponBase::OnReloadFinished() {
	bReloading = false;
}

void AWeaponBase::DoFire() {

}

void AWeaponBase::DoStopFiring() {

}

bool AWeaponBase::CanFire() const {
	return (!bReloading && !bFiring && AmmoLoaded > 0);
}

int32 AWeaponBase::GetDamage() const {
	return Cast<AProjectileBase>(ProjectileClass)->GetDamage();
}
