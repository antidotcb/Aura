// Copyright © 2024 antidotcb


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/Interactive.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	// SplineComponent->SetHiddenInGame(true);
}

void AAuraPlayerController::AutoRun()
{
	constexpr auto World = ESplineCoordinateSpace::World;

	if (APawn* ControlledPawn = GetPawn(); bAutoRunning && ControlledPawn != nullptr)
	{
		TimePassed += GetWorld()->GetDeltaSeconds();

		const FVector ActorLocation{ControlledPawn->GetActorLocation().X, ControlledPawn->GetActorLocation().Y, 0};
		const FVector ClosestSplineLocation = Spline->FindLocationClosestToWorldLocation(ActorLocation, World);
		const FVector LocationOnSpline{ClosestSplineLocation.X, ClosestSplineLocation.Y, 0};

		const float DistanceDifference = AutorunMaxSplineDistance - AutorunMinSplineDistance;
		const float DistanceFromActorToSpline = FVector::Dist(LocationOnSpline, ActorLocation);
		const float DistanceToSpline = FMath::Clamp(DistanceFromActorToSpline, 0.f, AutorunMaxSplineDistance);

		const bool bWithinSpline = DistanceToSpline < AutorunMinSplineDistance;

		const FVector DirectionOnSpline = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, World);
		const FVector DirectionToSpline = (LocationOnSpline - ActorLocation).GetSafeNormal();

		const float Scale = bWithinSpline ? 0.0f : (DistanceToSpline - AutorunMinSplineDistance) / DistanceDifference;
		const float CubicScale = FMath::Pow(FMath::Clamp(Scale, 0.0f, 1.0f), 2.0f);

		const FVector Direction = DirectionOnSpline * (1 - CubicScale) + DirectionToSpline * CubicScale;

		const auto Distance = FVector::Dist(ActorLocation, CachedDestination);
		const bool bIsCloseEnough = Distance < AutorunMaxSplineDistance;
		const bool bIsRunningAway = Distance > PrevDistanceToDestination;
		const bool bShouldStop = bIsCloseEnough && bIsRunningAway && !bWithinSpline;

		if (bShouldStop || Distance < AutoRunAcceptanceRadius)
		{
			Spline->ClearSplinePoints();
			bAutoRunning = false;
			return;
		}

		const float Speed = FMath::Clamp(1.0f - CubicScale, 0.2f, 1.0f);
		ControlledPawn->AddMovementInput(Direction.GetSafeNormal() * Speed);

		PrevDistanceToDestination = Distance;
	}
}

void AAuraPlayerController::PlayerTick(const float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRun();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext != nullptr);

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

	check(InputConfig != nullptr);

	if (InputConfig != nullptr)
	{
		AuraInputComponent->BindAbilityActions(InputConfig, this,
		                                       &AAuraPlayerController::AbilityInputTagPressed,
		                                       &AAuraPlayerController::AbilityInputTagReleased,
		                                       &AAuraPlayerController::AbilityInputTagHeld);
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	bAutoRunning = false;
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (auto* const ControlledPawn = GetPawn<APawn>(); ControlledPawn != nullptr)
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (!HitResult.bBlockingHit) return;

	LastInteractive = CurrentInteractive;
	CurrentInteractive = Cast<IInteractive>(HitResult.GetActor());

	if (LastInteractive != CurrentInteractive)
	{
		if (LastInteractive != nullptr)
			LastInteractive->UnhighlightActor();
		if (CurrentInteractive != nullptr)
			CurrentInteractive->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(const FGameplayTag Tag)
{
	if (Tag.MatchesTagExact(FAuraGameplayTags::InputTag_LMB))
	{
		bTargetting = CurrentInteractive != nullptr;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::LeftMouseButtonHoldMove()
{
	FollowTime += GetWorld()->GetDeltaSeconds();

	if (!HitResult.bBlockingHit) return;
	CachedDestination = HitResult.ImpactPoint;
	CachedDestination.Z = 0;

	APawn* ControlledPawn = GetPawn<APawn>();
	if (ControlledPawn == nullptr) return;

	const FVector Direction = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
	ControlledPawn->AddMovementInput(Direction);
}

void AAuraPlayerController::AbilityInputTagHeld(const FGameplayTag Tag)
{
	if (Tag.MatchesTagExact(FAuraGameplayTags::InputTag_LMB) && !bTargetting)
	{
		LeftMouseButtonHoldMove();
		return;
	}

	UAuraAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr) return;

	AbilitySystemComponent->AbilityInputTagHeld(Tag);
}

void AAuraPlayerController::AbilityInputTagReleased(const FGameplayTag Tag)
{
	UAuraAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	if (!Tag.MatchesTagExact(FAuraGameplayTags::InputTag_LMB) || bTargetting)
	{
		if (AbilitySystemComponent == nullptr) return;

		AbilitySystemComponent->AbilityInputTagReleased(Tag);
		return;
	}

	FollowTime = 0;
	bAutoRunning = false;

	if (FollowTime < ShortPressThreshold)
	{
		const UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(
			this, GetPawn<APawn>()->GetActorLocation(), CachedDestination);

		if (Path == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Path is null"));
			return;
		}

		check(Path != nullptr);

		Spline->ClearSplinePoints();
		for (const auto& Point : Path->PathPoints)
		{
			Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
		}

		bAutoRunning = !Path->PathPoints.IsEmpty();
		if (bAutoRunning)
		{
			PrevDistanceToDestination = FVector::Dist(GetPawn<APawn>()->GetActorLocation(), CachedDestination);
			CachedDestination = Path->PathPoints.Last();
			CachedDestination.Z = 0;
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAbilitySystemComponent()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		UAbilitySystemComponent* Asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>());
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(Asc);
	}

	return AuraAbilitySystemComponent;
}
