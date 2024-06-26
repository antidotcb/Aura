// Copyright © 2024 antidotcb


#include "AuraInputConfig.h"

const UInputAction* UAuraInputConfig::GetActionByTag(const FGameplayTag& Tag, const bool bLogNotFound) const
{
	for (const auto& [InputAction, InputTag] : InputActions)
	{
		if (InputAction && InputTag.MatchesTag(Tag))
		{
			return InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("No input action found for tag %s in %s"), *Tag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
