// Copyright © 2024 antidotcb


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");

	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::InitializePrimaryAttributes() const
{
	if (DefaultPrimaryAttributeEffect == nullptr)
	{
		return;
	}

	check(GetAbilitySystemComponent() != nullptr);

	const auto& Effect = DefaultPrimaryAttributeEffect;

	const auto EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	const auto EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(Effect, 1.0f, EffectContext);

	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}
