// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ProtagonistHUD.generated.h"

/**
 * 
 */
UCLASS()
class GAMEOFF2021_API AProtagonistHUD : public AHUD {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	class UUserWidget* CurrentWidget;

public:
	AProtagonistHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;
};
