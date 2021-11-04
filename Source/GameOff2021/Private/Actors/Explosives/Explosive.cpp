// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Explosives/Explosive.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Interfaces/DamageableInterface.h"

AExplosive::AExplosive() {
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName("BlockAllDynamic");
	Mesh->bCastDynamicShadow = true;
	Mesh->bAffectDynamicIndirectLighting = true;
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCanEverAffectNavigation(false);

	ExplosionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Explosion Collider"));
	ExplosionCollider->SetupAttachment(RootComponent);
	ExplosionCollider->SetCollisionProfileName("OverlapAllDynamic");
	ExplosionCollider->SetGenerateOverlapEvents(false);
	ExplosionCollider->SetCanEverAffectNavigation(false);
	ExplosionCollider->OnComponentBeginOverlap.AddDynamic(this, &AExplosive::OnOverlapBegin);
}

void AExplosive::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if(bExploding) {
		if(ExplosionLifeTime == 0) bExploding = false;
		else {
			Elapsed += DeltaTime;
			Progress = Elapsed / ExplosionLifeTime;
			if(Progress >= 1.0f) {
				Progress = 0.0f;
				Elapsed = 0.0f;
				bExploding = false;
			}
		}
	}
}

void AExplosive::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if(bExploding) {
		const auto Damageable = Cast<IDamageableInterface>(OtherActor);
		if(Damageable) {
			Damageable->NotifyDamage(this, OtherActor, Damage);
		}
	}
}

void AExplosive::Explode() {
	DoExplode();
}

void AExplosive::DoExplode() {
	ExplosionCollider->SetGenerateOverlapEvents(true);
	bExploding = true;

	if(ExplosionVFX->IsValid()) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionVFX, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f)); // Trust in bAutoDestroy we do ..for now
	}
}

float AExplosive::GetRadius() const {
	return ExplosionCollider->GetScaledSphereRadius();
}
