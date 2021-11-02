// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/ProtagonistController.h"
#include "Characters/ProtagonistCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AProtagonistController::AProtagonistController() {
	bShowMouseCursor = true;
}

void AProtagonistController::BeginPlay() {
	
}

void AProtagonistController::OnPossess(class APawn* InPawn) {
	Super::OnPossess(InPawn);
	if(InPawn->IsA(AProtagonistCharacter::StaticClass())) Protagonist = Cast<AProtagonistCharacter>(InPawn);
}

void AProtagonistController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AProtagonistController::TogglePauseMenu);
}

void AProtagonistController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
	LookAtMouse();
}

void AProtagonistController::LookAtMouse() {
	ensure(Protagonist);

	FVector MouseLocation, MouseDirection, MouseLocationEnd;
	FHitResult HitResult;

	DeprojectMousePositionToWorld(MouseLocation, MouseDirection);

	MouseLocationEnd = (MouseDirection * 10000) + MouseLocation;

	FCollisionQueryParams TraceSettings;
	FCollisionResponseParams TraceResponses;

	if(GetWorld()->LineTraceSingleByChannel(HitResult, MouseLocation, MouseLocationEnd, ECC_MAX, TraceSettings, TraceResponses)) {
		ProtagonistRotation = UKismetMathLibrary::FindLookAtRotation(Protagonist->GetActorLocation(), HitResult.ImpactPoint);
		Protagonist->SetActorRotation(FRotator(GetControlRotation().Pitch, ProtagonistRotation.Yaw, GetControlRotation().Roll));
	}
	else {
		Protagonist->SetActorRotation(FRotator(GetControlRotation().Pitch, ProtagonistRotation.Yaw, GetControlRotation().Roll));
	}
}

void AProtagonistController::TogglePauseMenu() {
	bPaused = !bPaused;

	UGameplayStatics::SetGamePaused(this, bPaused);
	bPaused ? SetInputMode(FInputModeUIOnly()) : SetInputMode(FInputModeGameOnly());
}
