// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProtagonistHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

AProtagonistHUD::AProtagonistHUD() {
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FClassFinder<UUserWidget> HUDBP(TEXT("/Game/UI/WBP_ProtagonistHUD"));
	HUDWidgetClass = HUDBP.Class;
}

void AProtagonistHUD::DrawHUD() {
	Super::DrawHUD();
}

void AProtagonistHUD::BeginPlay() {
	Super::BeginPlay();
	if(HUDWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if(CurrentWidget) {
			CurrentWidget->AddToViewport();
		}
	}
}
