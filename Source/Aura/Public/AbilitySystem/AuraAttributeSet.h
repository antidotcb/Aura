// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() = default;

	//! The source actor that applied the effect
	UPROPERTY()
	AActor* SourceActor{nullptr};

	UPROPERTY()
	AController* SourceController{nullptr};

	UPROPERTY()
	ACharacter* SourceCharacter{nullptr};

	UPROPERTY()
	UAbilitySystemComponent* SourceAbilitySystemComponent{nullptr};

	//! The target actor that the effect was applied to

	UPROPERTY()
	AActor* TargetActor{nullptr};

	UPROPERTY()
	AController* TargetController{nullptr};

	UPROPERTY()
	ACharacter* TargetCharacter{nullptr};;

	UPROPERTY()
	UAbilitySystemComponent* TargetAbilitySystemComponent{nullptr};

	//! The effect context handle
	FGameplayEffectContextHandle EffectContextHandle;
};


struct FGameplayEffectModCallbackData;

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void
	PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	using GameplayAttributeGetFunc = FGameplayAttribute(*)();
	TMap<FGameplayTag, GameplayAttributeGetFunc> TagsToAttributes;

	/* Primary attributes */

	UPROPERTY(ReplicatedUsing=OnRep_Strength, BlueprintReadOnly, Category="Primary")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength)

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UPROPERTY(ReplicatedUsing=OnRep_Vigor, BlueprintReadOnly, Category="Primary")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor)

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;


	UPROPERTY(ReplicatedUsing=OnRep_Intelligence, BlueprintReadOnly, Category="Primary")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence)

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;


	UPROPERTY(ReplicatedUsing=OnRep_Resilience, BlueprintReadOnly, Category="Primary")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience)

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	/* Secondary attributes */

	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadOnly, Category="Vital attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UPROPERTY(ReplicatedUsing=OnRep_MaxMana, BlueprintReadOnly, Category="Vital attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UPROPERTY(ReplicatedUsing=OnRep_Armor, BlueprintReadOnly, Category="Secondary attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor)

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UPROPERTY(ReplicatedUsing=OnRep_ArmorPenetration, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration)

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UPROPERTY(ReplicatedUsing=OnRep_BlockChance, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance)

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UPROPERTY(ReplicatedUsing=OnRep_CriticalHitChance, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance)

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UPROPERTY(ReplicatedUsing=OnRep_CriticalHitDamage, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage)

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UPROPERTY(ReplicatedUsing=OnRep_CriticalHitResistance, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance)

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UPROPERTY(ReplicatedUsing=OnRep_HealthRegeneration, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration)

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UPROPERTY(ReplicatedUsing=OnRep_ManaRegeneration, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration)

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	/* Vital attributes */

	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="Vitals attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UPROPERTY(ReplicatedUsing=OnRep_Mana, BlueprintReadOnly, Category="Vital attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

private:
	static FEffectProperties ExtractEffectProperties(const FGameplayEffectModCallbackData& Data);
};
