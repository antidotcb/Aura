// Copyright © 2024 antidotcb


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	StrengthDef.AttributeToCapture = UAuraAttributeSet::GetStrengthAttribute();
	StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrengthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
	RelevantAttributesToCapture.Add(StrengthDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor{0};
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(0.0f, Vigor);

	float Strength{0};
	GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluationParameters, Strength);

	const ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetEffectContext().GetSourceObject());
	check(CombatInterface != nullptr);
	const int32 PlayerLevel = CombatInterface ? CombatInterface->GetInGameLevel() : 1;

	return 80.0f + (Vigor * 2.5f) + (Strength * 0.5f) + (PlayerLevel * 10.0f);
}

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	StrengthDef.AttributeToCapture = UAuraAttributeSet::GetStrengthAttribute();
	StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrengthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
	RelevantAttributesToCapture.Add(StrengthDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence{0};
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max<float>(0.0f, Intelligence);

	float Strength{0};
	GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluationParameters, Strength);

	const ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetEffectContext().GetSourceObject());
	check(CombatInterface != nullptr);
	const int32 PlayerLevel = CombatInterface ? CombatInterface->GetInGameLevel() : 1;

	return 50.0f + (Intelligence * 3.5f) - (Strength * 0.5f) + (PlayerLevel * 15.0f);
}
