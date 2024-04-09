// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


class USplineComponent;
class UAuraInputConfig;
class IInteractive;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAuraAbilitySystemComponent;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	void AutoRun();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag Tag);
	void AbilityInputTagHeld(FGameplayTag Tag);
	void AbilityInputTagReleased(FGameplayTag Tag);

	UAuraAbilitySystemComponent* GetAbilitySystemComponent() ;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	IInteractive* CurrentInteractive;
	IInteractive* LastInteractive;

	FVector CachedDestination;

	float FollowTime{0};
	float ShortPressThreshold{0.3f};
	bool bAutoRunning{false};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float AutoRunAcceptanceRadius{50.0f};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	bool bTargetting{false};
};

