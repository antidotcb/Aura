// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData Health;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData MaxHealth;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UPROPERTY(ReplicatedUsing=OnRep_MaxMana, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData Mana;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UPROPERTY(ReplicatedUsing=OnRep_MaxMana, BlueprintReadOnly, Category="Vitals")
	FGameplayAttributeData MaxMana;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
};
