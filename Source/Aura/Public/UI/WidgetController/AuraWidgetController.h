// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() = default;

	FWidgetControllerParams(APlayerController* InPlayerController, APlayerState* InPlayerState,
	                        UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
		: PlayerController(InPlayerController)
		  , PlayerState(InPlayerState)
		  , AbilitySystemComponent(InAbilitySystemComponent)
		  , AttributeSet(InAttributeSet)
	{
	}

	APlayerController* PlayerController{nullptr};
	APlayerState* PlayerState{nullptr};
	UAbilitySystemComponent* AbilitySystemComponent{nullptr};
	UAttributeSet* AttributeSet{nullptr};
};


UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& InWidgetControllerParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialDataUpdate();

	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToDependencies();

	UFUNCTION(BlueprintCallable)
	virtual void UnBindCallbacksToDependencies();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAttributeSet> AttributeSet;
};
