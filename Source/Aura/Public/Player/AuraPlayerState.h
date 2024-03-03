// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class AURA_API AAuraPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
