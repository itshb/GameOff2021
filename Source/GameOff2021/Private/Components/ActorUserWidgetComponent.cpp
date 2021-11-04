// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ActorUserWidgetComponent.h"
#include "UI/Widgets/ActorUserWidget.h"

UActorUserWidgetComponent::UActorUserWidgetComponent() {
	SetDrawAtDesiredSize(true);
	SetWidgetSpace(EWidgetSpace::Screen);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UActorUserWidgetComponent::InitWidget() {
	Super::InitWidget();

	if(GetWidget()) {
#if !UE_BUILD_SHIPPING
		if(!GetWidget()->IsA(UActorUserWidget::StaticClass())) {
			UE_LOG(LogTemp, Warning, TEXT("WidgetClass for %s does not derive from UActorUserWidget"), *GetNameSafe(GetOwner()));
		}
#endif
		UActorUserWidget* instance = Cast<UActorUserWidget>(GetWidget());
		if(instance) {
			instance->SetOwningActor(GetOwner());
		}
	}

}
