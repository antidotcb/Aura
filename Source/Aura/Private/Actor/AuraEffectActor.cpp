// Copyright © 2024 antidotcb


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Character/AuraCharacter.h"
#include "Components/SphereComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AAuraEffectActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	const auto* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (AbilitySystemInterface == nullptr) return;

	const auto* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr) return;

	auto* AuraAttributeSet = Cast<UAuraAttributeSet>(AbilitySystemComponent->GetAttributeSet(UAuraAttributeSet::StaticClass()));
	if (AuraAttributeSet == nullptr) return;

	// TODO: This is a hack, we should use a proper way to apply damage
	auto* MutableAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);
	MutableAuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth() + 25.0f);

	Destroy();
}

void AAuraEffectActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlapEnd);
}
