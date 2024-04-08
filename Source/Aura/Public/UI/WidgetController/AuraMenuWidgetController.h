// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraMenuWidgetController.generated.h"


struct FGameplayTag;
struct FOnAttributeChangeData;
struct FAuraAttributeInfo;
class UAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, const FAuraAttributeInfo&, AttributeInfo);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialDataUpdate() override;

	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAttributeInfo> AttributeInfoDataAsset;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChanged OnAttributeChanged;

protected:
	void AttributeChanged(const FOnAttributeChangeData& Data) const;

private:
	void BroadcastAttributeChange(const FGameplayTag& Tag, float Value) const;
};
