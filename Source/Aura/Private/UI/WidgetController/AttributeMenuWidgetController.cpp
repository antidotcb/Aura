// Copyright © 2024 antidotcb


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"




void UAttributeMenuWidgetController::BroadcastAttributeChange(const FGameplayTag& Tag, const float Value) const
{
	check(AttributeInfoDataAsset);
	const FAuraAttributeInfo Info = AttributeInfoDataAsset->GetAttributeInfo(Tag, Value, true);
	if (Info.AttributeTag.IsValid())
	{
		OnAttributeChanged.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialDataUpdate()
{
	auto* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);


	for (const auto& AttributeInfo : AuraAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeChange(AttributeInfo.Key, AttributeInfo.Value().GetNumericValue(AuraAttributeSet));
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const auto* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AbilitySystemComponent);

	for (const auto& AttributeInfo : AuraAttributeSet->TagsToAttributes)
	{
		const FGameplayTag AttributeTag = AttributeInfo.Key;
		const FGameplayAttribute GameplayAttribute = AttributeInfo.Value();

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GameplayAttribute).AddLambda(
			[this, AttributeTag](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeChange(AttributeTag, Data.NewValue);
			});
	}
}
