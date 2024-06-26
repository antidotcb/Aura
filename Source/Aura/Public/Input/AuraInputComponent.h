// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <typename UserClass, typename PressedFuncType, typename ReleaseFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
	                        ReleaseFuncType ReleasedFunc, HeldFuncType HeldFunc)
	{
		check(InputConfig);

		for (const auto& [InputAction, InputTag] : InputConfig->InputActions)
		{
			if (InputAction && InputTag.IsValid())
			{
				if (PressedFunc)
				{
					BindAction(InputAction, ETriggerEvent::Started, Object, PressedFunc, InputTag);
				}

				if (ReleasedFunc)
				{
					BindAction(InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputTag);
				}

				if (HeldFunc)
				{
					BindAction(InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputTag);
				}
			}
		}
	}
};
