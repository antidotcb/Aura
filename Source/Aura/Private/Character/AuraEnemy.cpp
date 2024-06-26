// Copyright © 2024 antidotcb


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraEnemy::AAuraEnemy()
{
	if (GetMesh() != nullptr)
	{
		GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));

	InitializeAttributes();
}

int32 AAuraEnemy::GetInGameLevel() const
{
	return Level;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->AbilityActorInfoSet();

}

void AAuraEnemy::HighlightMesh(UMeshComponent* MeshComponent)
{
	if (MeshComponent == nullptr)
	{
		return;
	}

	MeshComponent->SetRenderCustomDepth(true);
	MeshComponent->SetCustomDepthStencilValue(250);
}

void AAuraEnemy::UnhighlightMesh(UMeshComponent* MeshComponent)
{
	if (MeshComponent == nullptr)
	{
		return;
	}

	MeshComponent->SetRenderCustomDepth(false);
	MeshComponent->SetCustomDepthStencilValue(0);
}

void AAuraEnemy::HighlightActor()
{
	// GEngine->AddOnScreenDebugMessage(100, 5.f, FColor::Red, FString::Printf(TEXT("Highlighting actor: %s"), *GetName()));

	Highlighted = true;

	HighlightMesh(GetMesh());
	HighlightMesh(Weapon);
}

void AAuraEnemy::UnhighlightActor()
{
	// GEngine->AddOnScreenDebugMessage(100, 5.f, FColor::Greed, FString::Printf(TEXT("Stop highlighting actor: %s"), *GetName()));

	Highlighted = false;

	UnhighlightMesh(GetMesh());
	UnhighlightMesh(Weapon);
}
