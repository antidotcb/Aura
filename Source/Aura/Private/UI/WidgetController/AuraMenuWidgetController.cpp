// Copyright © 2024 antidotcb


#include "UI/WidgetController/AuraMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"


void UAttributeMenuWidgetController::AttributeChanged(const FOnAttributeChangeData& Data) const
{
	const FGameplayAttribute& GameplayAttribute = Data.Attribute;
	// Data.Attribute.
}

void UAttributeMenuWidgetController::BroadcastAttributeChange(const FGameplayTag& Tag, const float Value) const
{
	check(AttributeInfoDataAsset);
	const FAuraAttributeInfo Info = AttributeInfoDataAsset->GetAttributeInfo(Tag, Value, true);
	if (Info.AttributeTag.IsValid())
	{
		OnAttributeChanged.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialDataUpdate()
{
	const auto* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AuraAttributeSet);

	// Health
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Vital_Health, AuraAttributeSet->GetHealth());
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Secondary_MaxHealth, AuraAttributeSet->GetMaxHealth());
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Secondary_HealthRegeneration,
	                         AuraAttributeSet->GetHealthRegeneration());

	// Mana
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Vital_Mana, AuraAttributeSet->GetMana());
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Secondary_MaxMana, AuraAttributeSet->GetMaxMana());
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Secondary_ManaRegeneration,
	                         AuraAttributeSet->GetManaRegeneration());


	// Primary
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Primary_Strength, AuraAttributeSet->GetStrength());
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Primary_Intelligence, AuraAttributeSet->GetIntelligence());
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Primary_Resilience, AuraAttributeSet->GetResilience());
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Primary_Vigor, AuraAttributeSet->GetVigor());

	// Secondary
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Secondary_Armor, AuraAttributeSet->GetArmor());
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Secondary_ArmorPenetration,
	                         AuraAttributeSet->GetArmorPenetration());
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Secondary_BlockChance, AuraAttributeSet->GetBlockChance());
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Secondary_CriticalChance,
	                         AuraAttributeSet->GetCriticalHitChance());
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Secondary_CriticalDamage,
	                         AuraAttributeSet->GetCriticalHitDamage());
	BroadcastAttributeChange(FAuraGameplayTags::Attributes_Secondary_CriticalResistance,
	                         AuraAttributeSet->GetCriticalHitResistance());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const auto* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AbilitySystemComponent);

	const auto HealthAttribute = AuraAttributeSet->GetHealthAttribute();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttribute)
	                      .AddUObject(this, &UAttributeMenuWidgetController::AttributeChanged);
}
