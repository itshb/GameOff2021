// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ThrowComponent.h"
#include "Actors/Explosives/Grenade.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

UThrowComponent::UThrowComponent() {
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
	bAutoRegister = true;
	
	ForceMultiplier = 1.0f;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

	LaunchPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Launch Point"));
}

void UThrowComponent::BeginPlay() {
	Super::BeginPlay();

	CalculateTrajectory();
}

void UThrowComponent::DisplayTrajectory(bool bState) {
	if(!Mesh) {
		UE_LOG(LogTemp, Warning, TEXT("No mesh provided for trajectory %s"), *GetFName().ToString());
	}

	if(Meshes.IsEmpty()) CalculateTrajectory();

	for(auto Child : Meshes) {
		Child->SetVisibility(bState);
	}
}

void UThrowComponent::CalculateTrajectory() {
	if(!Mesh) {
		UE_LOG(LogTemp, Warning, TEXT("No mesh provided for trajectory %s"), *GetFName().ToString());
	}

	if(!Meshes.IsEmpty()) {
		for(auto Child : Meshes) {
			Child->DestroyComponent();
		}

		Spline->ClearSplinePoints();
		Meshes.Empty();
	}

	FPredictProjectilePathParams Params = FPredictProjectilePathParams(1.0f, LaunchPoint->GetComponentLocation(), Velocity(), 1.0f, ECollisionChannel::ECC_WorldStatic, GetOwner());
	FPredictProjectilePathResult Result;

	UGameplayStatics::PredictProjectilePath(this, Params, Result);

	for(int i = 0; i < Result.PathData.Num() - 1; ++i) {
		Spline->AddSplinePointAtIndex(Result.PathData[i].Location, i, ESplineCoordinateSpace::Local, true);
	}

	for(int i = 0; i < Spline->GetNumberOfSplinePoints() - 1; ++i) {
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());

		SplineMesh->SetStaticMesh(Mesh);
		SplineMesh->SetMobility(EComponentMobility::Movable);
		SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMesh->RegisterComponentWithWorld(GetWorld());
		SplineMesh->AttachToComponent(LaunchPoint, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if(Material) SplineMesh->SetMaterial(0, Material);

		Meshes.Add(SplineMesh);

		const FVector StartPoint = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector EndPoint = Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		const FVector StartTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector EndTangent = Spline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		SplineMesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent);
		
		SplineMesh->SetVisibility(false);
	}
}

void UThrowComponent::ThrowGrenade(TSubclassOf<class AGrenade> Grenade) {
	const FVector Forward = GetOwner()->GetActorForwardVector();
	
	AGrenade* Spawned = GetWorld()->SpawnActor<AGrenade>(Grenade, LaunchPoint->GetComponentLocation() + Forward, GetOwner()->GetActorRotation());
	Spawned->GetMesh()->SetSimulatePhysics(true);
	Spawned->GetMesh()->AddImpulse(Velocity(), NAME_None, true);
}

FVector UThrowComponent::Velocity() const {
	const FVector Forward = GetOwner()->GetActorForwardVector();
	const float Force = 1000.0f * ForceMultiplier;
	return FVector(Forward.X * Force, Forward.Y * Force, 200.0f * ForceMultiplier);
}

void UThrowComponent::UpdateForceMultiplier(const float Value) {
	if(Value == ForceMultiplier) return;

	ForceMultiplier = Value;
	if(ForceMultiplier > 2.0f) ForceMultiplier = 2.0f;
	if(ForceMultiplier < 1.0f) ForceMultiplier = 1.0f;
	
	CalculateTrajectory();
}
