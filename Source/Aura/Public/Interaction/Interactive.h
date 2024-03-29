// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactive.generated.h"

UINTERFACE(MinimalAPI)
class UInteractive : public UInterface
{
	GENERATED_BODY()
};


class AURA_API IInteractive
{
	GENERATED_BODY()

public:
	virtual void HighlightActor() = 0;
	virtual void UnhighlightActor() = 0;
};
