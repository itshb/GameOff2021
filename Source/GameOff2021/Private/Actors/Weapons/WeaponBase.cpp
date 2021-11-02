// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/WeaponBase.h"
#include "Actors/Projectiles/ProjectileBase.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

AWeaponBase::AWeaponBase() {
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
	if(!CanReload()) return;
	if(bFiring) StopFiring();
	bReloading = true;

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::OnReloadFinished, ReloadSpeed, false);
}

void AWeaponBase::OnReloadFinished() {
	bReloading = false;

	if(AmmoRemaining < MagazineSize) {
		AmmoLoaded = AmmoRemaining;
		AmmoRemaining = 0;
	}
	else {
		AmmoLoaded = 20;
		AmmoRemaining -= 20;
	}
}

void AWeaponBase::DoFire() {
	if(ProjectileClass) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, MuzzleLocation->GetComponentLocation(), GetAttachParentActor()->GetActorRotation(), ActorSpawnParams);

		--AmmoLoaded;

		if(AmmoLoaded == 0) {
			StopFiring();
		}
	}
}

void AWeaponBase::DoStopFiring() {

}

bool AWeaponBase::CanFire() const {
	return (!bReloading && !bFiring && AmmoLoaded > 0);
}

bool AWeaponBase::CanReload() const {
	return (!bReloading && AmmoRemaining > 0);
}

int32 AWeaponBase::GetDamage() const {
	return Cast<AProjectileBase>(ProjectileClass)->GetDamage();
}

void AWeaponBase::AttachTo(class AActor* ParentActor, const FName& SocketName) {
	AttachToActor(ParentActor, FAttachmentTransformRules::KeepRelativeTransform);
}
