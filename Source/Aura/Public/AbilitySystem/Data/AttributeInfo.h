// Copyright © 2024 antidotcb

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttributeValue{0};
};

UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	FAuraAttributeInfo GetAttributeInfo(const FGameplayTag& AttributeTag, bool bLogNotFound) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributesInformation;
};
