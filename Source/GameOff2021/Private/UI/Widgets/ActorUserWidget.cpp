// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/ActorUserWidget.h"

void UActorUserWidget::SetOwningActor(class AActor* NewOwner) {
	if(OwningActor == NewOwner) return;

	OwningActor = NewOwner;
	OnOwningActorChanged.Broadcast(NewOwner);
}
