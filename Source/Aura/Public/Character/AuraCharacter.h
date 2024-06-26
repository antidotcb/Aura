// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacter.generated.h"

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* NewController) override;

	// ~ Begin ICombatInterface Interface.

	virtual int32 GetInGameLevel() const override;

	// ~ End ICombatInterface Interface.

private:
	void InitAbilityActorInfo();
};
