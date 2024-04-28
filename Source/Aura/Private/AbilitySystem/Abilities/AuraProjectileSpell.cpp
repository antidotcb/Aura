// Copyright © 2024 antidotcb


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UKismetSystemLibrary::PrintString(this, TEXT("ActivateAbility (C++)"), true, false, FLinearColor::Red, 5.0f);

	if (!HasAuthority(&ActivationInfo)) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	if (CombatInterface != nullptr && ProjectileClass != nullptr)
	{
		// TODO: Set the projectile rotation to the same as the owner

		const FVector Translation = CombatInterface->GetCombatSocketLocation();
		const FTransform SpawnTransform = FTransform(Translation);
		AActor* OwningActor = GetOwningActorFromActorInfo();
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass, SpawnTransform, OwningActor,
			Cast<APawn>(OwningActor),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// TODO: Give a projectile a Gameplay Effect Spec for the damage it will deal

		Projectile->FinishSpawning(SpawnTransform);
	}
}
