// Copyright © 2024 antidotcb


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

#include "Character/AuraCharacter.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* Task = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return Task;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();

	const APlayerController* AuraPlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (AuraPlayerController == nullptr) return;

	FHitResult HitResult;
	AuraPlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	OnMouseTargetData.Broadcast(HitResult.Location);
}
