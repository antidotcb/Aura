// Copyright © 2024 antidotcb


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FGameplayTag FAuraGameplayTags::Attributes_Primary_Strength;
FGameplayTag FAuraGameplayTags::Attributes_Primary_Resilience;
FGameplayTag FAuraGameplayTags::Attributes_Primary_Intelligence;
FGameplayTag FAuraGameplayTags::Attributes_Primary_Vigor;

FGameplayTag FAuraGameplayTags::Attributes_Secondary_Armor;
FGameplayTag FAuraGameplayTags::Attributes_Secondary_ArmorPenetration;
FGameplayTag FAuraGameplayTags::Attributes_Secondary_BlockChance;
FGameplayTag FAuraGameplayTags::Attributes_Secondary_CriticalChance;
FGameplayTag FAuraGameplayTags::Attributes_Secondary_CriticalDamage;
FGameplayTag FAuraGameplayTags::Attributes_Secondary_CriticalResistance;
FGameplayTag FAuraGameplayTags::Attributes_Secondary_MaxHealth;
FGameplayTag FAuraGameplayTags::Attributes_Secondary_HealthRegeneration;
FGameplayTag FAuraGameplayTags::Attributes_Secondary_MaxMana;
FGameplayTag FAuraGameplayTags::Attributes_Secondary_ManaRegeneration;

FGameplayTag FAuraGameplayTags::Attributes_Vital_Mana;
FGameplayTag FAuraGameplayTags::Attributes_Vital_Health;

FGameplayTag FAuraGameplayTags::InputTag_LMB;
FGameplayTag FAuraGameplayTags::InputTag_RMB;
FGameplayTag FAuraGameplayTags::InputTag_1;
FGameplayTag FAuraGameplayTags::InputTag_2;
FGameplayTag FAuraGameplayTags::InputTag_3;
FGameplayTag FAuraGameplayTags::InputTag_4;


void FAuraGameplayTags::InitializeGameplayTags()
{
	// Primary
	Attributes_Primary_Strength = RegisterTag("Attributes.Primary.Strength", "");
	Attributes_Primary_Resilience = RegisterTag("Attributes.Primary.Resilience", "");
	Attributes_Primary_Intelligence = RegisterTag("Attributes.Primary.Intelligence", "");
	Attributes_Primary_Vigor = RegisterTag("Attributes.Primary.Vigor", "");

	// Secondary
	Attributes_Secondary_Armor = RegisterTag("Attributes.Secondary.Armor", "");
	Attributes_Secondary_ArmorPenetration = RegisterTag("Attributes.Secondary.ArmorPenetration", "");
	Attributes_Secondary_BlockChance = RegisterTag("Attributes.Secondary.BlockChance", "");
	Attributes_Secondary_CriticalChance = RegisterTag("Attributes.Secondary.CriticalChance", "");
	Attributes_Secondary_CriticalDamage = RegisterTag("Attributes.Secondary.CriticalDamage", "");
	Attributes_Secondary_CriticalResistance = RegisterTag("Attributes.Secondary.CriticalResistance", "");

	// Health
	Attributes_Vital_Health = RegisterTag("Attributes.Vital.Health", "");
	Attributes_Secondary_MaxHealth = RegisterTag("Attributes.Secondary.MaxHealth", "");
	Attributes_Secondary_HealthRegeneration = RegisterTag("Attributes.Secondary.HealthRegeneration", "");

	// Mana
	Attributes_Vital_Mana = RegisterTag("Attributes.Vital.Mana", "");
	Attributes_Secondary_MaxMana = RegisterTag("Attributes.Secondary.MaxMana", "");
	Attributes_Secondary_ManaRegeneration = RegisterTag("Attributes.Secondary.ManaRegeneration", "");

	// Input
	InputTag_LMB = RegisterTag("InputTag.LMB", "");
	InputTag_RMB = RegisterTag("InputTag.RMB", "");
	InputTag_1 = RegisterTag("InputTag.1", "");
	InputTag_2 = RegisterTag("InputTag.2", "");
	InputTag_3 = RegisterTag("InputTag.3", "");
	InputTag_4 = RegisterTag("InputTag.4", "");
}


FGameplayTag FAuraGameplayTags::RegisterTag(const char* TagName, const char* Comment)
{
	return UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(Comment));
}
