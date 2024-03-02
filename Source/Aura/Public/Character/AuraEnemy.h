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
	AAuraEnemy();

	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;

	UFUNCTION(BlueprintPure, Category="Properties")
	bool IsHighlighted() const { return Highlighted; }

private:
	static void HighlightMesh(UMeshComponent* MeshComponent);
	static void UnhighlightMesh(UMeshComponent* MeshComponent);

	bool Highlighted{false};
};
