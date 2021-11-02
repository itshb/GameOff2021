// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/DamageableInterface.h"

void IDamageableInterface::NotifyDamage(class AActor* Instigator, class AActor* Recipient, const int32 Amount) {
	if(Recipient->GetClass()->ImplementsInterface(UDamageableInterface::StaticClass())) Cast<IDamageableInterface>(Recipient)->TakeDamage(Instigator, Amount);
}
