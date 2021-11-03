// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Explosives/Explosive.h"

AExplosive::AExplosive() {
	PrimaryActorTick.bCanEverTick = true;
}

void AExplosive::BeginPlay() {
	Super::BeginPlay();
	
}

void AExplosive::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

