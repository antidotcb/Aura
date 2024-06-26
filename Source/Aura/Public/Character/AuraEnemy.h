// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/Interactive.h"
#include "AuraEnemy.generated.h"

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IInteractive
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	//~ Begin IInteractive Interface.

	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;

	UFUNCTION(BlueprintPure, Category="Properties")
	bool IsHighlighted() const { return Highlighted; }

	//~ End IInteractive Interface.

	// ~ Begin ICombatInterface Interface.

	virtual int32 GetInGameLevel() const override;

	// ~ End ICombatInterface Interface.

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level{1};

private:
	static void HighlightMesh(UMeshComponent* MeshComponent);
	static void UnhighlightMesh(UMeshComponent* MeshComponent);

	bool Highlighted{false};
};
