// Copyright © 2024 antidotcb


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnEffectAppliedToSelf);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
	                                 FString::Printf(
		                                 TEXT("Tag: %s"), *FAuraGameplayTags::Attributes_Secondary_Armor.ToString()));
}

void UAuraAbilitySystemComponent::OnEffectAppliedToSelf(UAbilitySystemComponent* AbilitySystemComponent,
                                                        const FGameplayEffectSpec& EffectSpec,
                                                        FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer SourceTags;
	EffectSpec.GetAllAssetTags(SourceTags);

	//EffectAssetTags.Broadcast(SourceTags);
}
