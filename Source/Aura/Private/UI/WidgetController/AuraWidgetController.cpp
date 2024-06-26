// Copyright © 2024 antidotcb


#include "UI/WidgetController/AuraWidgetController.h"


void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InWidgetControllerParams)
{
	PlayerController = InWidgetControllerParams.PlayerController;
	PlayerState = InWidgetControllerParams.PlayerState;
	AbilitySystemComponent = InWidgetControllerParams.AbilitySystemComponent;
	AttributeSet = InWidgetControllerParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialDataUpdate()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}

void UAuraWidgetController::UnBindCallbacksToDependencies()
{
}
