// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/Weapon.h"
#include "Actors/Projectiles/Projectile.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

AWeapon::AWeapon() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->bCastDynamicShadow = true;
	Mesh->bAffectDynamicIndirectLighting = true;
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCanEverAffectNavigation(false);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	MuzzleLocation->SetupAttachment(RootComponent);

	FireRate = 0.1f;
	ReloadSpeed = 1.0f;
	AmmoMax = 120;
	AmmoRemaining = 120;
	MagazineSize = 20;
	AmmoLoaded = MagazineSize;
}

void AWeapon::Fire() {
	if(!CanFire()) return;
	bFiring = true;

	if(FireRate == 0) {
		DoFire();
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AWeapon::DoFire, FireRate, true, 0.0f);
}

void AWeapon::DoFire() {
	if(ProjectileClass) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation->GetComponentLocation(), GetAttachParentActor()->GetActorRotation(), ActorSpawnParams);

		--AmmoLoaded;

		if(AmmoLoaded == 0) {
			StopFiring();
		}

		OnWeaponFired.Broadcast(AmmoLoaded);
	}
}

void AWeapon::StopFiring() {
	if(!bFiring) return;
	bFiring = false;

	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void AWeapon::Reload() {
	if(!CanReload()) return;
	if(bFiring) StopFiring();
	bReloading = true;

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeapon::OnReloadFinished, ReloadSpeed, false);

	OnReloadStarted.Broadcast(ReloadSpeed);
}

void AWeapon::StopReloading() {
	if(!bReloading) return;
	bReloading = false;
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	OnReloadCancelled.Broadcast();
}

void AWeapon::OnReloadFinished() {
	bReloading = false;

	if(AmmoRemaining < MagazineSize) {
		AmmoLoaded = AmmoRemaining;
		AmmoRemaining = 0;
	}
	else {
		AmmoRemaining -= (MagazineSize - AmmoLoaded);
		AmmoLoaded = MagazineSize;
	}

	OnAmmoUpdated.Broadcast(AmmoLoaded, AmmoRemaining);
}

void AWeapon::AddAmmo(const int32 Value) {
	if(AmmoRemaining == AmmoMax || Value == 0) return;

	(Value < 0) ? AmmoRemaining -= Value : AmmoRemaining += Value;

	if(AmmoRemaining > AmmoMax) AmmoRemaining = AmmoMax;

	OnAmmoUpdated.Broadcast(AmmoLoaded, AmmoRemaining);
}

bool AWeapon::CanFire() const {
	return (!bReloading && !bFiring && AmmoLoaded > 0);
}

bool AWeapon::CanReload() const {
	return (!bReloading && AmmoRemaining > 0 && AmmoLoaded != MagazineSize);
}

int32 AWeapon::GetDamage() const {
	return Cast<AProjectile>(ProjectileClass)->GetDamage();
}

void AWeapon::AttachTo(class AActor* ParentActor, const FName& SocketName) {
	AttachToActor(ParentActor, FAttachmentTransformRules::KeepRelativeTransform);
}
