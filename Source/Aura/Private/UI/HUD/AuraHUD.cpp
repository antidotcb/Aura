// Copyright © 2024 antidotcb


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(Params);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetMenuWidgetController(const FWidgetControllerParams& Params)
{
	if (AuraMenuWidgetController == nullptr)
	{
		AuraMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AuraMenuWidgetController->SetWidgetControllerParams(Params);
		AuraMenuWidgetController->BindCallbacksToDependencies();
	}

	return AuraMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* InPlayerController, APlayerState* InPlayerState,
                           UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
{
	checkf(OverlayWidgetClass != nullptr, TEXT("OverlayWidgetClass is not set in %s"), *GetName());
	checkf(OverlayWidgetControllerClass != nullptr, TEXT("OverlayWidgetControllerClass is not set in %s"), *GetName());

	const FWidgetControllerParams Params{InPlayerController, InPlayerState, InAbilitySystemComponent, InAttributeSet};

	auto* Widget = CreateWidget(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	auto* WidgetController = GetOverlayWidgetController(Params);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialDataUpdate();

	OverlayWidget->AddToViewport();
}
