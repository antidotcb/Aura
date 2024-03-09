// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"


class UGameplayEffect;


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


UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FActiveGameplayEffectHandle ApplyEffectToTarget(AActor* TargetActor,
	                                                TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	bool RemoveInstantEffect(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);


	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	bool bDestroyActorOnEffectApplication = false;

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	bool bDestroyActorOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy{EEffectApplicationPolicy::DoNotApply};

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy{EEffectApplicationPolicy::DoNotApply};

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy{EEffectApplicationPolicy::DoNotApply};

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy{EEffectRemovalPolicy::RemoveOnEndOverlap};

private:
	TMap<uint32, FActiveGameplayEffectHandle> ActiveInfiniteEffects;
};
