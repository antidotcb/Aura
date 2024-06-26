// Copyright © 2024 antidotcb


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::SpawnProjectile(const FGameplayAbilityActivationInfo ActivationInfo, const FVector& TargetLocation)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (AvatarActor == nullptr) return;
	if (!AvatarActor->HasAuthority()) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(AvatarActor);

	if (CombatInterface != nullptr && ProjectileClass != nullptr)
	{
		const FVector SpawnLocation = CombatInterface->GetCombatSocketLocation();
		const FQuat Rotation = FQuat::FindBetween(TargetLocation, SpawnLocation);

		const FTransform SpawnTransform = FTransform(Rotation, SpawnLocation);
		AActor* OwningActor = GetOwningActorFromActorInfo();
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass, SpawnTransform, OwningActor,
			Cast<APawn>(OwningActor),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// TODO: Give a projectile a Gameplay Effect Spec for the damage it will deal

		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
