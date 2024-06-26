// Copyright © 2024 antidotcb


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Primary_Strength,
	                     &UAuraAttributeSet::GetStrengthAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Primary_Resilience,
	                     &UAuraAttributeSet::GetResilienceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Primary_Intelligence,
	                     &UAuraAttributeSet::GetIntelligenceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Primary_Vigor,
	                     &UAuraAttributeSet::GetVigorAttribute);

	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Vital_Health,
	                     &UAuraAttributeSet::GetHealthAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Secondary_MaxHealth,
	                     &UAuraAttributeSet::GetMaxHealthAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Secondary_HealthRegeneration,
	                     &UAuraAttributeSet::GetHealthRegenerationAttribute);

	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Vital_Mana,
	                     &UAuraAttributeSet::GetManaAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Secondary_MaxMana,
	                     &UAuraAttributeSet::GetMaxManaAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Secondary_ManaRegeneration,
	                     &UAuraAttributeSet::GetManaRegenerationAttribute);

	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Secondary_Armor,
	                     &UAuraAttributeSet::GetArmorAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Secondary_ArmorPenetration,
	                     &UAuraAttributeSet::GetArmorPenetrationAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Secondary_BlockChance,
	                     &UAuraAttributeSet::GetBlockChanceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Secondary_CriticalChance,
	                     &UAuraAttributeSet::GetCriticalHitChanceAttribute);

	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Secondary_CriticalDamage,
	                     &UAuraAttributeSet::GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Attributes_Secondary_CriticalResistance,
	                     &UAuraAttributeSet::GetCriticalHitResistanceAttribute);
}

FEffectProperties UAuraAttributeSet::ExtractEffectProperties(const FGameplayEffectModCallbackData& Data)
{
	FEffectProperties Result{};

	Result.EffectContextHandle = Data.EffectSpec.GetContext();

	{
		Result.SourceAbilitySystemComponent = Result.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

		const auto& SourceAbilityActorInfo = Result.SourceAbilitySystemComponent->AbilityActorInfo;

		if (IsValid(Result.SourceAbilitySystemComponent) && SourceAbilityActorInfo.IsValid()
			&& SourceAbilityActorInfo->AvatarActor.IsValid())
		{
			Result.SourceActor = SourceAbilityActorInfo->AvatarActor.Get();
			Result.SourceController = SourceAbilityActorInfo->PlayerController.Get();
			if (Result.SourceController == nullptr && Result.SourceActor != nullptr)
			{
				if (const APawn* Pawn = Cast<APawn>(Result.SourceActor))
				{
					Result.SourceController = Cast<APlayerController>(Pawn->GetController());
				}
			}
			if (Result.SourceController != nullptr)
			{
				Result.SourceCharacter = Cast<ACharacter>(Result.SourceController->GetPawn());
			}
		}
	}

	{
		Result.TargetActor = Data.Target.GetAvatarActor();
		const auto& TargetAbilityActorInfo = Data.Target.AbilityActorInfo;
		if (TargetAbilityActorInfo.IsValid() && TargetAbilityActorInfo->AvatarActor.IsValid())
		{
			Result.TargetActor = TargetAbilityActorInfo->AvatarActor.Get();
			Result.TargetController = TargetAbilityActorInfo->PlayerController.Get();
			if (Result.TargetController == nullptr && Result.TargetActor != nullptr)
			{
				if (const APawn* Pawn = Cast<APawn>(Result.TargetActor))
				{
					Result.TargetController = Cast<APlayerController>(Pawn->GetController());
				}
			}
			if (Result.TargetController != nullptr)
			{
				Result.TargetCharacter = Cast<ACharacter>(Result.TargetController->GetPawn());
			}
			const auto TargetAbilitySystemComponent = TargetAbilityActorInfo->AbilitySystemComponent.Get();
			Result.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
				Result.TargetActor);
			check(TargetAbilitySystemComponent == Result.TargetAbilitySystemComponent);
		}
	}

	return Result;
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		const auto CurrentHealth = GetHealth();

		const auto Percent = OldValue > 0.0f ? CurrentHealth / OldValue : 1.0f;
		SetHealth(NewValue * Percent);
	}

	if (Attribute == GetMaxManaAttribute())
	{
		const auto CurrentMana = GetMana();

		const auto Percent = OldValue > 0.0f ? CurrentMana / OldValue : 1.0f;
		SetMana(NewValue * Percent);
	}
}

void UAuraAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
                                                float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, CriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}
