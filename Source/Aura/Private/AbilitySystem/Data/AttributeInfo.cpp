// Copyright © 2024 antidotcb


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::GetAttributeInfo(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const auto& AttributeInfo : AttributesInformation)
	{
		if (AttributeInfo.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return AttributeInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("AttributeInfo for tag %s not found on AttributeInfo [%s]."),
		       *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return {};
}
