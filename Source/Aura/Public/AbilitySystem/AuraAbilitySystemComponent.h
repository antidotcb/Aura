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

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:
	static void OnEffectAppliedToSelf(UAbilitySystemComponent* AbilitySystemComponent,
	                                  const FGameplayEffectSpec& EffectSpec,
	                                  FActiveGameplayEffectHandle ActiveEffectHandle);


};
