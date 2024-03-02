// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/Interactive.h"
#include "AuraEnemy.generated.h"

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IInteractive
{
	GENERATED_BODY()

public:
	void HighlightActor() override;
	void UnhighlightActor() override;
};
