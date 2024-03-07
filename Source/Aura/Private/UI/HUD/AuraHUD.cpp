// Copyright © 2024 antidotcb


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	auto* Widget = CreateWidget(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}
