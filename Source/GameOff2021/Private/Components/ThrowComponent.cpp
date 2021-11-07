// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ThrowComponent.h"
#include "Actors/Explosives/Grenade.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

UThrowComponent::UThrowComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	
	ForceMultiplier = 1.0f;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

	/*Mesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("Mesh"));
	//Mesh->SetupAttachment(Spline);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetVisibility(false);
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Mesh->SetMobility(EComponentMobility::Movable);*/
	//Mesh->SetStaticMesh();
	//Mesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
}

void UThrowComponent::BeginPlay() {
	Super::BeginPlay();
}

void UThrowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UThrowComponent::DisplayTrajectory(bool bState) {
	if(!Mesh) return;

	if(bState) {
		FPredictProjectilePathParams Params = FPredictProjectilePathParams(1.0f, GetOwner()->GetActorLocation(), Velocity(), 1.0f, ECollisionChannel::ECC_WorldStatic, GetOwner());
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
			SplineMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			if(Material) SplineMesh->SetMaterial(0, Material);

			Meshes.Add(SplineMesh);

			const FVector StartPoint = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			const FVector EndPoint = Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
			const FVector StartTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
			const FVector EndTangent = Spline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);	

			SplineMesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent);
		}
	}
	else {
		for(auto Child : Meshes) {
			Child->DestroyComponent();
		}
		Spline->ClearSplinePoints();
		Meshes.Empty();
	}
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
