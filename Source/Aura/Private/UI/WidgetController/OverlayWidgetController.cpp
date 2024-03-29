// Copyright © 2024 antidotcb


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"


void UOverlayWidgetController::BroadcastInitialDataUpdate()
{
	const auto* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const auto* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AbilitySystemComponent);

	const auto HealthAttribute = AuraAttributeSet->GetHealthAttribute();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttribute)
	                      .AddUObject(this, &UOverlayWidgetController::HealthChanged);

	const auto MaxHealthAttribute = AuraAttributeSet->GetMaxHealthAttribute();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxHealthAttribute)
	                      .AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	const auto ManaAttribute = AuraAttributeSet->GetManaAttribute();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ManaAttribute)
	                      .AddUObject(this, &UOverlayWidgetController::ManaChanged);

	const auto MaxManaAttribute = AuraAttributeSet->GetMaxManaAttribute();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxManaAttribute)
	                      .AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
