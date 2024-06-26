// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FAuraGameplayTags
{
	static FGameplayTag RegisterTag(const char* TagName, const char* Comment);
	static void InitializeGameplayTags();

	static FGameplayTag Attributes_Primary_Strength;
	static FGameplayTag Attributes_Primary_Resilience;
	static FGameplayTag Attributes_Primary_Intelligence;
	static FGameplayTag Attributes_Primary_Vigor;

	static FGameplayTag Attributes_Secondary_Armor;
	static FGameplayTag Attributes_Secondary_ArmorPenetration;
	static FGameplayTag Attributes_Secondary_BlockChance;
	static FGameplayTag Attributes_Secondary_CriticalChance;
	static FGameplayTag Attributes_Secondary_CriticalDamage;
	static FGameplayTag Attributes_Secondary_CriticalResistance;
	static FGameplayTag Attributes_Secondary_MaxHealth;
	static FGameplayTag Attributes_Secondary_HealthRegeneration;
	static FGameplayTag Attributes_Secondary_MaxMana;
	static FGameplayTag Attributes_Secondary_ManaRegeneration;

	static FGameplayTag Attributes_Vital_Mana;
	static FGameplayTag Attributes_Vital_Health;


	static FGameplayTag InputTag_LMB;
	static FGameplayTag InputTag_RMB;
	static FGameplayTag InputTag_1;
	static FGameplayTag InputTag_2;
	static FGameplayTag InputTag_3;
	static FGameplayTag InputTag_4;

private:
};
