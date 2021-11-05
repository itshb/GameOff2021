// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ThrowComponent.h"
#include "Actors/Explosives/Grenade.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UThrowComponent::UThrowComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	
	ForceMultiplier = 1.0f;
}

void UThrowComponent::BeginPlay() {
	Super::BeginPlay();
}

void UThrowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bRenderTrajectory) RenderTrajectory();
}

void UThrowComponent::DisplayTrajectory(bool bState) {
	// TODO: Implement
	bRenderTrajectory = bState;
	if(!bState) return;
}

void UThrowComponent::RenderTrajectory() const {
	FPredictProjectilePathParams Params = FPredictProjectilePathParams(1.0f, GetOwner()->GetActorLocation(), Velocity(), 1.0f);
	FPredictProjectilePathResult Result;

	UGameplayStatics::PredictProjectilePath(this, Params, Result);
	//for(const auto& Segment : Result.PathData) {
		//DrawDebugBox(GetWorld(), Segment.Location, FVector(2.0f, 2.0f, 2.0f), FColor::Purple, false, 5.0f);
	//}
}

void UThrowComponent::ThrowGrenade(TSubclassOf<class AGrenade> Grenade) {
	const FVector OwnerLoc = GetOwner()->GetActorLocation();
	const FVector Forward = GetOwner()->GetActorForwardVector() * 20.0f;
	const FTransform SpawnTransform = FTransform(GetOwner()->GetActorRotation(), Forward + OwnerLoc, FVector(1.0, 1.0f, 1.0f));
	
	AGrenade* Spawned = GetWorld()->SpawnActor<AGrenade>(Grenade, OwnerLoc + Forward, GetOwner()->GetActorRotation());
	Spawned->GetMesh()->SetSimulatePhysics(true);
	Spawned->GetMesh()->AddImpulse(Velocity(), NAME_None, true);
}

FVector UThrowComponent::Velocity() const {
	const FVector Forward = GetOwner()->GetActorForwardVector();
	const float Force = 1000.0f * ForceMultiplier;
	return FVector(Forward.X * Force, Forward.Y * Force, 200.0f * ForceMultiplier);
}
