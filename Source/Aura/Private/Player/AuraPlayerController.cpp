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
	static float TimePassed = 0;

	if (APawn* ControlledPawn = GetPawn(); bAutoRunning && ControlledPawn != nullptr)
	{
		TimePassed += GetWorld()->GetDeltaSeconds();

		const FVector& ActorLocation = ControlledPawn->GetActorLocation();
		const FVector& LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ActorLocation, ESplineCoordinateSpace::World);
		const float DistanceToSpline = FVector::Dist(LocationOnSpline, ActorLocation);

		const bool bOnPath = DistanceToSpline < 100;
		const FVector DirectionOnSpline = Spline->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);
		const FVector DirectionToSpline = LocationOnSpline - ActorLocation;
		const FVector Direction = bOnPath
			                          ? DirectionOnSpline
			                          : DirectionOnSpline.GetSafeNormal() + DirectionToSpline.GetSafeNormal();

		const float DistanceToDestination = FVector::Dist(LocationOnSpline, CachedDestination);
		const bool bReachedDestination = DistanceToDestination < AutoRunAcceptanceRadius;
		if (TimePassed > 0.1 || bReachedDestination)
		{
			const FColor SphereColor = bOnPath ? FColor::Black : FColor::Yellow;
			const FColor LineColor = bOnPath ? FColor::White : FColor::Red;
			const int LifeTime = !bOnPath ? 8 : 5;
			DrawDebugSphere(GetWorld(), LocationOnSpline, 5, 36, SphereColor, false, LifeTime);
			const FVector LineStart = LocationOnSpline + FVector(0, 0, 10);
			const FVector LineEnd = LineStart + Direction.GetSafeNormal() * 100;
			DrawDebugLine(GetWorld(), LineStart, LineEnd, LineColor, false, LifeTime);
			TimePassed = 0;
		}

		ControlledPawn->AddMovementInput(Direction.GetSafeNormal());


		if (bReachedDestination)
		{
			Spline->ClearSplinePoints();
			bAutoRunning = false;
		}
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
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (!HitResult.bBlockingHit)
	{
		return;
	}

	LastInteractive = CurrentInteractive;
	CurrentInteractive = Cast<IInteractive>(HitResult.GetActor());

	if (LastInteractive == CurrentInteractive)
	{
		return;
	}

	if (LastInteractive != nullptr)
	{
		LastInteractive->UnhighlightActor();
	}

	if (CurrentInteractive != nullptr)
	{
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

void AAuraPlayerController::AbilityInputTagHeld(const FGameplayTag Tag)
{
	UAuraAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	if (!Tag.MatchesTagExact(FAuraGameplayTags::InputTag_LMB) || bTargetting)
	{
		if (AbilitySystemComponent == nullptr) return;

		AbilitySystemComponent->AbilityInputTagHeld(Tag);
		return;
	}

	FollowTime += GetWorld()->GetDeltaSeconds();
	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		CachedDestination = Hit.ImpactPoint;
	}

	APawn* ControlledPawn = GetPawn<APawn>();
	if (ControlledPawn == nullptr) return;

	const FVector Direction = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
	ControlledPawn->AddMovementInput(Direction);
}

void AAuraPlayerController::AbilityInputTagReleased(const FGameplayTag Tag)
{
	UAuraAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	if (!Tag.MatchesTagExact(FAuraGameplayTags::InputTag_LMB))
	{
		if (AbilitySystemComponent == nullptr) return;

		AbilitySystemComponent->AbilityInputTagReleased(Tag);
		return;
	}

	if (bTargetting) return;

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
		int n = 1;
		for (const auto& Point : Path->PathPoints)
		{
			Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);

			if (n == 1)
			{
				DrawDebugSphere(GetWorld(), Point, 15, 36, FColor::Green, false, 1);
			}
			else if (n == Path->PathPoints.Num())
			{
				DrawDebugSphere(GetWorld(), Point, 15, 36, FColor::Black, false, 1);
				CachedDestination = Point;
			}
			else
			{
				DrawDebugSphere(GetWorld(), Point, 10, 36, FColor::White, false, 1);
			}
			n++;
		}

		bAutoRunning = true;
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
