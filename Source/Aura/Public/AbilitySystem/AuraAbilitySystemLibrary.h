// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class AAuraHUD;
struct FWidgetControllerParams;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;


UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(
		BlueprintPure,
		Category = "AuraAbilitySystemLibrary|WidgetController",
		meta = (WorldContext = "WorldContextObject")
	)
	static UOverlayWidgetController* GetOverlayWidgetController(UObject* WorldContextObject);

	UFUNCTION(
		BlueprintPure,
		Category = "AuraAbilitySystemLibrary|WidgetController",
		meta = (WorldContext = "WorldContextObject")
	)
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(UObject* WorldContextObject);

private:
	static std::pair<FWidgetControllerParams, AAuraHUD*> GetWidgetControllerParams(const UObject* WorldContextObject);
};
