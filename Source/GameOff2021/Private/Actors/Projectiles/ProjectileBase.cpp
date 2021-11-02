// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectiles/ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/DamageableInterface.h"

AProjectileBase::AProjectileBase() {
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RootComponent = Collider;
	Collider->InitSphereRadius(2.0f);
	Collider->BodyInstance.SetCollisionProfileName("Projectile");
	Collider->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	Collider->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	Collider->CanCharacterStepUpOn = ECB_No;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = Collider;
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 1.0f;

	DamageAmount = 1;
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	const auto Damageable = Cast<IDamageableInterface>(OtherActor);
	if(Damageable) {
		Damageable->NotifyDamage(this, OtherActor, DamageAmount);
		Destroy();
	}
}
