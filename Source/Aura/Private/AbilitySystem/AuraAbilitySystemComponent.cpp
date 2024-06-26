// Copyright © 2024 antidotcb


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddStatic(&UAuraAbilitySystemComponent::OnEffectAppliedToSelf);
}

void UAuraAbilitySystemComponent::OnEffectAppliedToSelf(UAbilitySystemComponent* AbilitySystemComponent,
                                                        const FGameplayEffectSpec& EffectSpec,
                                                        FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer SourceTags;
	EffectSpec.GetAllAssetTags(SourceTags);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const auto& Ability : StartupAbilities)
	{
		if (Ability)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);

			const auto* AuraGameplayAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability);
			if (AuraGameplayAbility && AuraGameplayAbility->InputTag.IsValid())
			{
				AbilitySpec.DynamicAbilityTags.AddTag(AuraGameplayAbility->InputTag);
				GiveAbility(AbilitySpec);
			}
			else
			{
				GiveAbilityAndActivateOnce(AbilitySpec);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}
