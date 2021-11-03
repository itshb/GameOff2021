// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthUpdated, const int32, Current, const int32, Max, const float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEOFF2021_API UAttributesComponent : public UActorComponent {
	GENERATED_BODY()

	int32 Health;

	int32 HealthMax;

	bool bInitialized;

protected:
	virtual void BeginPlay() override;

	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthUpdated OnHealthUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UAttributesComponent();

	void Initialize(const int32 MaxHealth, const int32 CurrentHealth = 0);

	void Heal(const int32 Amount);
	
	void Damage(const int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	int32 GetHealth() const { return Health; }
	
	UFUNCTION(BlueprintCallable)
	int32 GetHealthMax() const { return HealthMax; }
	
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const { return static_cast<float>(Health) / HealthMax; }
};
