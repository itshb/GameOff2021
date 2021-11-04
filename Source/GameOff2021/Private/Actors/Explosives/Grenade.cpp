// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Explosives/Grenade.h"

AGrenade::AGrenade() {
	DetonationDelay = 1.0f;
}

void AGrenade::BeginPlay() {
	Super::BeginPlay();

	if(bDetonateOnImpact) {
		GetMesh()->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);
	}
	else {
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AGrenade::Explode, DetonationDelay);
	}
}

void AGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	Explode();
}
