// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageableInterface : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEOFF2021_API IDamageableInterface {
	GENERATED_BODY()

protected:
	virtual void TakeDamage(class AActor* Instigator, const int32 Amount) = 0;

public:
	void NotifyDamage(class AActor* Instigator, class AActor* Recipient, const int32 Amount);
};
