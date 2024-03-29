// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"


class UGameplayEffect;

UENUM(BlueprintType)
enum class EDestroyActorPolicy
{
	DoNotDestroy,
	DestroyOnEffectApplication,
	DestroyOnEffectRemoval
};

UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	DoNotApply,
	ApplyOnOverlap,
	ApplyOnEndOverlap
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	DoNotRemove,
	RemoveOnEndOverlap
};

USTRUCT(BlueprintType)
struct FEffectWithPolicies
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectApplicationPolicy ApplicationPolicy{EEffectApplicationPolicy::ApplyOnOverlap};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectRemovalPolicy RemovalPolicy{EEffectRemovalPolicy::RemoveOnEndOverlap};
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor,
	                         TSubclassOf<UGameplayEffect> GameplayEffectClass,
	                         const EEffectRemovalPolicy RemovalPolicy = EEffectRemovalPolicy::DoNotRemove);

	UFUNCTION(BlueprintCallable)
	bool RemoveInstantEffects(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	EDestroyActorPolicy DestroyActorPolicy{EDestroyActorPolicy::DoNotDestroy};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<FEffectWithPolicies> Effects;

private:
	TMap<uint32, TArray<FActiveGameplayEffectHandle>> ActiveInfiniteEffects;
};
