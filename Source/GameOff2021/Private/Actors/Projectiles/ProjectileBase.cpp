// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectiles/ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/DamageableInterface.h"

AProjectileBase::AProjectileBase() {
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RootComponent = Collider;
	Collider->InitSphereRadius(5.0f);
	Collider->BodyInstance.SetCollisionProfileName("Projectile");
	Collider->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlapBegin);
	Collider->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	Collider->CanCharacterStepUpOn = ECB_No;
	Collider->SetEnableGravity(false);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = Collider;
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 1.0f;

	Damage = 1;
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	Destroy();
	OnDestroy();
}

void AProjectileBase::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	const auto Damageable = Cast<IDamageableInterface>(OtherActor);
	if(Damageable) {
		Damageable->NotifyDamage(this, OtherActor, Damage);
		if(bDestroyOnOverlap) {
			Destroy();
			OnDestroy();
		}
	}
}
