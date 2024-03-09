// Copyright © 2024 antidotcb


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

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

FActiveGameplayEffectHandle AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor,
                                                                  const TSubclassOf<UGameplayEffect>
                                                                  GameplayEffectClass)
{
	UAbilitySystemComponent* AbilitySystemComponent =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (AbilitySystemComponent == nullptr) return {};

	auto EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const auto EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		GameplayEffectClass, 1.0f, EffectContextHandle);
	const auto& EffectSpec = *EffectSpecHandle.Data.Get();
	const auto Result = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(EffectSpec);

	if (EffectSpec.Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite)
	{
		const auto UID = TargetActor->GetUniqueID();
		ActiveInfiniteEffects.Add(UID, Result);
	}

	return Result;
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (const auto* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		AbilitySystemComponent == nullptr)
	{
		return;
	}

	auto bHasAppliedEffect = false;

	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap
		&& InstantGameplayEffectClass != nullptr)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
		bHasAppliedEffect = true;
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap
		&& DurationGameplayEffectClass != nullptr)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
		bHasAppliedEffect = true;
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap
		&& InfiniteGameplayEffectClass != nullptr)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
		bHasAppliedEffect = true;
	}

	if (bHasAppliedEffect && bDestroyActorOnEffectApplication
		&& !((InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap
				|| InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
			&& InfiniteGameplayEffectClass != nullptr)
		&& !(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap
			&& InstantGameplayEffectClass != nullptr)
		&& !(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap
			&& DurationGameplayEffectClass != nullptr))
	{
		Destroy();
	}
}

bool AAuraEffectActor::RemoveInstantEffect(AActor* TargetActor)
{
	auto* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (AbilitySystemComponent == nullptr) return false;

	const auto UID = TargetActor->GetUniqueID();
	if (!ActiveInfiniteEffects.Contains(UID)) return false;

	if (!AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveInfiniteEffects[UID])) return false;

	ActiveInfiniteEffects.Remove(UID);
	return true;
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (const auto* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		AbilitySystemComponent == nullptr)
	{
		return;
	}

	auto bNeedToDestroy = false;


	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap
		&& InstantGameplayEffectClass != nullptr)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);

		bNeedToDestroy |= bDestroyActorOnEffectApplication;
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap
		&& DurationGameplayEffectClass != nullptr)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);

		bNeedToDestroy |= bDestroyActorOnEffectApplication;
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap
		&& InfiniteGameplayEffectClass != nullptr)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);

		bNeedToDestroy |= bDestroyActorOnEffectApplication;
	}


	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap
		&& InfiniteGameplayEffectClass != nullptr)
	{
		RemoveInstantEffect(TargetActor);

		bNeedToDestroy |= bDestroyActorOnEffectRemoval;
	}

	if (bNeedToDestroy)
	{
		Destroy();
	}
}
