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
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	/* Vital attributes */

	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UPROPERTY(ReplicatedUsing=OnRep_MaxMana, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UPROPERTY(ReplicatedUsing=OnRep_MaxMana, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

private:
	static FEffectProperties ExtractEffectProperties(const FGameplayEffectModCallbackData& Data);
};
