// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributesComponent.h"

UAttributesComponent::UAttributesComponent() {
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
}

void UAttributesComponent::BeginPlay() {
	Super::BeginPlay();

	if(Health == 0) OnDeath.Broadcast();
}

/*void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}*/

void UAttributesComponent::Initialize(const int32 MaxHealth, const int32 CurrentHealth) {
	if(bInitialized) return;
	(MaxHealth < 0) ? HealthMax = FMath::Abs(MaxHealth) : HealthMax = MaxHealth;
	(CurrentHealth == 0) ? Health = HealthMax : Health = CurrentHealth;
	bInitialized = true;
}

void UAttributesComponent::Heal(const int32 Amount) {
	if(Health == HealthMax) return;

	(Amount > 0) ? Health + Amount : Health - Amount;

	if(Health > HealthMax) Health = HealthMax;

	OnHealthUpdated.Broadcast(Health, HealthMax, Health / HealthMax);
}

void UAttributesComponent::Damage(const int32 Amount) {
	(Amount > 0) ? Health - Amount : Health + Amount;
	
	if(Health <= 0) {
		Health = 0;
		OnDeath.Broadcast();
	}
}
