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
FGameplayTag FAuraGameplayTags::Attributes_Secondary_MaxMana;

FGameplayTag FAuraGameplayTags::Attributes_Vital_Mana;
FGameplayTag FAuraGameplayTags::Attributes_Vital_Health;


FGameplayTag FAuraGameplayTags::RegisterTag(const char* TagName, const char* Comment)
{
	return UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(Comment));
}

void FAuraGameplayTags::InitializeGameplayTags()
{
	Attributes_Primary_Strength = RegisterTag("Attributes.Primary.Strength",
	                                          "Increases physical damage and lowers mana, increases health");
	Attributes_Primary_Resilience = RegisterTag("Attributes.Primary.Resilience",
	                                            "Increases Armor and Armor Penetration");
	Attributes_Primary_Intelligence = RegisterTag("Attributes.Primary.Intelligence",
	                                              "Increases magical damage and mana, mana regeneration");
	Attributes_Primary_Vigor = RegisterTag("Attributes.Primary.Vigor", "Increases health, health regeneration");

	Attributes_Secondary_Armor = RegisterTag("Attributes.Secondary.Armor", "Lowers physical damage taken");
	Attributes_Secondary_ArmorPenetration = RegisterTag("Attributes.Secondary.ArmorPenetration",
	                                                    "Ignores a percentage of the target's armor");
	Attributes_Secondary_BlockChance = RegisterTag("Attributes.Secondary.BlockChance",
	                                               "Chance to block half of incoming damage");
	Attributes_Secondary_CriticalChance = RegisterTag("Attributes.Secondary.CriticalChance",
	                                                  "Chance to deal critical damage");
	Attributes_Secondary_CriticalDamage = RegisterTag("Attributes.Secondary.CriticalDamage",
	                                                  "Critical damage multiplier");
	Attributes_Secondary_CriticalResistance = RegisterTag("Attributes.Secondary.CriticalResistance",
	                                                      "Chance to resist critical damage");
	Attributes_Secondary_MaxHealth = RegisterTag("Attributes.Secondary.MaxHealth", "Maximum health points");
	Attributes_Secondary_MaxMana = RegisterTag("Attributes.Secondary.MaxMana", "Maximum mana points");

	Attributes_Vital_Health = RegisterTag("Attributes.Vital.Health", "Health points");
	Attributes_Vital_Mana = RegisterTag("Attributes.Vital.Mana", "Mana points");
}
