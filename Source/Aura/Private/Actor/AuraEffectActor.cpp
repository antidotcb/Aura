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

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass) const
{
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (AbilitySystemComponent == nullptr) return;

	auto EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const auto EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		GameplayEffectClass, 1.0f, EffectContextHandle);
	const auto& EffectSpec = *EffectSpecHandle.Data.Get();
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(EffectSpec);
}
