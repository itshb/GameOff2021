// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DefaultGameMode.h"
#include "Characters/ProtagonistCharacter.h"
#include "Controllers/ProtagonistController.h"
#include "UI/ProtagonistHUD.h"

ADefaultGameMode::ADefaultGameMode() {
	DefaultPawnClass = AProtagonistCharacter::StaticClass();
	PlayerControllerClass = AProtagonistController::StaticClass();
	HUDClass = AProtagonistHUD::StaticClass();
}
