// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UGameplayAbility;

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	static void OnEffectAppliedToSelf(UAbilitySystemComponent* AbilitySystemComponent,
	                                  const FGameplayEffectSpec& EffectSpec,
	                                  FActiveGameplayEffectHandle ActiveEffectHandle);

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
};
