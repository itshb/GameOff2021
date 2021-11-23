// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/AICharacterBase.h"
#include "AI/Controllers/AIControllerBase.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/NavMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Characters/ProtagonistCharacter.h"

AAICharacterBase::AAICharacterBase() {
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000.0f;
	PawnSensingComp->HearingThreshold = 600.0f;
	PawnSensingComp->LOSHearingThreshold = 1200.0f;

	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f, false);
	GetCapsuleComponent()->SetCapsuleRadius(42.0f);

	GetMovementComponent()->NavAgentProps.AgentRadius = 42;
	GetMovementComponent()->NavAgentProps.AgentHeight = 192;
}

void AAICharacterBase::BeginPlay() {
	Super::BeginPlay();
	
	if(PawnSensingComp) {
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AAICharacterBase::OnSeePlayer);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AAICharacterBase::OnHearNoise);
	}
}

void AAICharacterBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AAICharacterBase::OnSeePlayer(class APawn* Pawn) {
	if(Pawn->IsA(AProtagonistCharacter::StaticClass())) {
		LastSeenTime = GetWorld()->GetTimeSeconds();
		bSensedTarget = true;

		auto* AIController = Cast<AAIControllerBase>(Controller);
		if(AIController) AIController->SetTargetEnemy(Cast<AProtagonistCharacter>(Pawn));
	}
}

void AAICharacterBase::OnHearNoise(class APawn* PawnInstigator, const FVector& Location, float Volume) {
	
}

void AAICharacterBase::UseWeapon() {
	
}
