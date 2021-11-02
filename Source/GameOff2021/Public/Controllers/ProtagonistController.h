// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProtagonistController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEOFF2021_API AProtagonistController : public APlayerController {
	GENERATED_BODY()

	class AProtagonistCharacter* Protagonist;

	bool bPaused;

	FRotator ProtagonistRotation;

protected:
	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	virtual void SetupInputComponent() override;

	virtual void OnPossess(class APawn* InPawn) override;

	virtual void BeginPlay() override;

	virtual void PlayerTick(float DeltaTime) override;

	void LookAtMouse();

public:
	AProtagonistController();
};
