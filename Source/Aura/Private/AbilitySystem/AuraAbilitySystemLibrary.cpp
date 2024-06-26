// Copyright © 2024 antidotcb


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "UI/HUD/AuraHUD.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(UObject* WorldContextObject)
{
	auto [Params, AuraHUD] = GetWidgetControllerParams(WorldContextObject);
	if (!AuraHUD)
	{
		return nullptr;
	}

	return AuraHUD->GetOverlayWidgetController(Params);
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(UObject* WorldContextObject)
{
	auto [Params, AuraHUD] = GetWidgetControllerParams(WorldContextObject);
	if (!AuraHUD)
	{
		return nullptr;
	}

	return AuraHUD->GetMenuWidgetController(Params);
}

std::pair<FWidgetControllerParams, AAuraHUD*> UAuraAbilitySystemLibrary::GetWidgetControllerParams(
	const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			if (const AAuraPlayerState* AuraPlayerState = PlayerController->GetPlayerState<AAuraPlayerState>())
			{
				if (UAbilitySystemComponent* AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent())
				{
					const FWidgetControllerParams Params{
						PlayerController,
						PlayerController->PlayerState,
						AbilitySystemComponent,
						AuraPlayerState->GetAttributeSet()
					};

					return {Params, AuraHUD};
				}
			}
		}
	}
	return {{}, nullptr};
}
