// Copyright © 2024 antidotcb


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.0f);
	InitMaxHealth(100.0f);
	InitMana(50.0f);
	InitMaxMana(100.0f);
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
			auto TargetAbilitySystemComponent = TargetAbilityActorInfo->AbilitySystemComponent.Get();
			Result.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Result.TargetActor);
			check(TargetAbilitySystemComponent == Result.TargetAbilitySystemComponent);
		}
	}

	return Result;
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const auto EffectProperties = ExtractEffectProperties(Data);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		const auto OriginalValue = NewValue;
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
		return;
	}

	if (Attribute == GetManaAttribute())
	{
		const auto OriginalValue = NewValue;
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
		return;
	}
}

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		const auto OriginalValue = NewValue;
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
		return;
	}

	if (Attribute == GetManaAttribute())
	{
		const auto OriginalValue = NewValue;
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
		return;
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UAuraAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
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
