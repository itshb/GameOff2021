// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorUserWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwningActorChanged, AActor*, NewOwner);

/**
 * Widget used to retrieve owning actor information for use inside blueprints
 * E.g. get current health for display on floating healthbars or damage taken for visual display above the actor
 */
UCLASS()
class GAMEOFF2021_API UActorUserWidget : public UUserWidget {
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = Actor)
	class AActor* OwningActor;

public:
	UFUNCTION(BlueprintCallable, Category = HUD)
	void SetOwningActor(class AActor* NewOwner);

	UPROPERTY(BlueprintAssignable, Category = HUD)
	FOnOwningActorChanged OnOwningActorChanged;
};
