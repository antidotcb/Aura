// Copyright © 2024 antidotcb


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	auto* SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneComponent);
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor,
                                           const TSubclassOf<UGameplayEffect> GameplayEffectClass,
                                           const EEffectRemovalPolicy RemovalPolicy)
{
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	check(AbilitySystemComponent != nullptr);

	auto EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const auto EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContextHandle);
	const auto& EffectSpec = *EffectSpecHandle.Data.Get();
	const auto EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(EffectSpec);

	const auto bIsInfiniteEffect = EffectSpec.Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

	if (!bIsInfiniteEffect || RemovalPolicy != EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		// If the effect is not infinite or if it is infinite but we don't want to remove it on end overlap - no need to track it
		return;
	}

	const auto ObjectID = TargetActor->GetUniqueID();
	if (!ActiveInfiniteEffects.Contains(ObjectID))
	{
		ActiveInfiniteEffects.Add(ObjectID, {});
	}

	auto& ActiveObjectInfiniteEffects = this->ActiveInfiniteEffects[ObjectID];
	if (!ActiveObjectInfiniteEffects.Contains(EffectHandle))
	{
		ActiveObjectInfiniteEffects.Add(EffectHandle);
	}
}

bool AAuraEffectActor::RemoveInstantEffects(AActor* TargetActor)
{
	auto* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	check(AbilitySystemComponent != nullptr);

	const auto ObjectID = TargetActor->GetUniqueID();
	if (!ActiveInfiniteEffects.Contains(ObjectID))
	{
		return false;
	}

	auto& ActiveObjectInfiniteEffects = this->ActiveInfiniteEffects[ObjectID];

	auto bHasRemovedEffect{false};
	for (const auto& EffectHandle : ActiveObjectInfiniteEffects)
	{
		bHasRemovedEffect |= AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle, 1);
	}

	ActiveInfiniteEffects.Remove(ObjectID);
	return bHasRemovedEffect;
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	const auto* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}

	auto bHasAppliedEffect = false;
	for (const auto& [GameplayEffectClass, ApplicationPolicy, RemovalPolicy] : Effects)
	{
		if (ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap && GameplayEffectClass != nullptr)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffectClass, RemovalPolicy);
			bHasAppliedEffect = true;
		}
	}

	if (bHasAppliedEffect && DestroyActorPolicy == EDestroyActorPolicy::DestroyOnEffectApplication)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (const auto* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		AbilitySystemComponent == nullptr)
	{
		return;
	}

	auto bHasAppliedEffect{false};
	for (const auto& [GameplayEffectClass, ApplicationPolicy, RemovalPolicy] : Effects)
	{
		if (ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap && GameplayEffectClass != nullptr)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffectClass, RemovalPolicy);
			bHasAppliedEffect |= true;
		}
	}

	const auto bHasRemovedEffect = RemoveInstantEffects(TargetActor);

	if ((bHasAppliedEffect && DestroyActorPolicy == EDestroyActorPolicy::DestroyOnEffectApplication) ||
		(bHasRemovedEffect && DestroyActorPolicy == EDestroyActorPolicy::DestroyOnEffectRemoval))
	{
		Destroy();
	}
}
