// Copyright © 2024 antidotcb


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnEffectAppliedToSelf);
}

void UAuraAbilitySystemComponent::OnEffectAppliedToSelf(UAbilitySystemComponent* AbilitySystemComponent,
                                                        const FGameplayEffectSpec& EffectSpec,
                                                        FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer SourceTags;
	EffectSpec.GetAllAssetTags(SourceTags);
}
