// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ActorUserWidgetComponent.generated.h"

/**
 * Component used as a wrapper for UActorUserWidget
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class GAMEOFF2021_API UActorUserWidgetComponent : public UWidgetComponent {
	GENERATED_BODY()

public:
	virtual void InitWidget() override;

	UActorUserWidgetComponent();
};
