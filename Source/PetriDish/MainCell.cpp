// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCell.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"    

#include "Enemy.h"
#include "Resource.h"

#include <cmath>

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"

// Sets default values
AMainCell::AMainCell()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainCell::BeginPlay()
{
	Super::BeginPlay();

    TArray<UCameraComponent*> cams;
    GetComponents<UCameraComponent>(cams);
    if (cams.Num() > 0)
    {
        CameraComponent = cams[0];
    }

    TArray<UFloatingPawnMovement*> moves;
    GetComponents<UFloatingPawnMovement>(moves);
    if (moves.Num() > 0)
    {
        MovementComponent = moves[0];
    }

    // Create the sphere collider
    SphereCollider = GetComponentByClass<UCapsuleComponent>();
    // SphereCollider->OnComponentBeginOverlap.Add(this, &AMainCell::OnCollisionBegin);
    SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AMainCell::OnCollisionBegin);

    // Add a movement component for simple floating-style movement
    MovementComponent = GetComponentByClass<UFloatingPawnMovement>();
    MovementComponent->UpdatedComponent = SphereCollider;

    EdroIdleAnimation = Cast<UPaperFlipbookComponent>(GetDefaultSubobjectByName(TEXT("edro")));
    BarierIdleAnimation = Cast<UPaperFlipbookComponent>(GetDefaultSubobjectByName(TEXT("barier")));
}

// Called every frame
void AMainCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// MoveToCursor();
    
    FVector Velocity = SphereCollider->GetPhysicsLinearVelocity();
    if (Velocity.Size() > MinSpeed) {
        SphereCollider->AddForce(-Velocity.GetSafeNormal() * FrictionForce);
    }

    FVector AngularVelocity = SphereCollider->GetPhysicsAngularVelocityInDegrees();
    auto RotationZSpeed = AngularVelocity.Z;
    if (FMath::Abs(RotationZSpeed) > MinRotationSpeed) {
        FVector Torque = FVector(0.0f, 0.0f, -FMath::Sign(AngularVelocity.Z) * RotateFrictionMult);
        SphereCollider->AddTorqueInRadians(Torque);
    }

    PlayMoveAnimation(Velocity.Size() > MinSpeed);

    SetActorLocation({ GetActorLocation().X, GetActorLocation().Y, 100. });
}

// Called to bind functionality to input
void AMainCell::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Add the input mapping context to the local player
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(_inputMappingContext, 0); // Priority 0
        }
    }

    if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Bind MoveForward
        {
            enhancedInputComponent->BindAction(_goForwardAction, ETriggerEvent::Triggered, this, &AMainCell::MoveForward);
        }

        // Bind MoveForward
        {
            enhancedInputComponent->BindAction(_goRightAction, ETriggerEvent::Triggered, this, &AMainCell::MoveRight);
        }

        // Bind MoveForward
        {
            enhancedInputComponent->BindAction(_rotateAction, ETriggerEvent::Triggered, this, &AMainCell::RorateRight);
        }
    }
}

void AMainCell::MoveForward(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();
    if (InputValue != 0.0f)
    {
        // Apply a force in the forward direction
        FVector Force = SphereCollider->GetForwardVector() * InputValue * MovementForce;
        SphereCollider->AddForce(Force);
    }
}

void AMainCell::MoveRight(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();
    if (InputValue != 0.0f)
    {
        // Apply a force in the right direction
        FVector Force = SphereCollider->GetRightVector() * InputValue * MovementForce;
        SphereCollider->AddForce(Force);
    }
}

void AMainCell::RorateRight(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();
    if (InputValue != 0.0f)
    {
        // Apply torque for rotation
        FVector Torque = FVector(0.0f, 0.0f, InputValue * RotationTorque);
        SphereCollider->AddTorqueInRadians(Torque);
    }
}

void AMainCell::PlayMoveAnimation(bool isMoving) {
    
    BarierIdleAnimation->SetFlipbook(BarierIdleAnimationClip);
    EdroIdleAnimation->SetFlipbook(EdroIdleAnimationClip);
    if (isMoving) {
        BarierIdleAnimation->Play();
        EdroIdleAnimation->Stop();
        EdroIdleAnimation->SetPlaybackPosition(0.0f, false);
    }
    else {
        EdroIdleAnimation->Play();
        BarierIdleAnimation->Stop();
        BarierIdleAnimation->SetPlaybackPosition(0.0f, false);
    }
}

void AMainCell::OnCollisionBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult) {
    
    if (OtherActor && (OtherActor != this))
    {
        {
            APawn* OtherPawn = Cast<AEnemy>(OtherActor);
            if (OtherPawn) {
                Damage();
            }
        }
        {
            APawn* OtherPawn = Cast<AResource>(OtherActor);
            if (OtherPawn) {
                OtherPawn->Destroy();
                Eat();
            }
        }
    }
}

void AMainCell::Eat() {
    CurrentAmount += 1;
    
    float mult = FMath::Square(CurrentAmount) / 100. + NormalScale;
    BarierIdleAnimation->SetRelativeScale3D({mult, mult, mult});
    SphereCollider->SetCapsuleRadius(FMath::Square(CurrentAmount) / 100. + NormalRadious);
    SphereCollider->SetMassOverrideInKg(NAME_None, FMath::Square(CurrentAmount) / 100. + 1, true);
    if (CurrentAmount > MaxAmmount) {
        //
    }
}

void AMainCell::Damage() {
    CurrentAmount = (int)ceil(CurrentAmount * 0.7);

    float mult = FMath::Square(CurrentAmount) / 100. + NormalScale;
    SphereCollider->SetCapsuleRadius(FMath::Square(CurrentAmount) / 100. + NormalRadious);
    SphereCollider->SetMassOverrideInKg(NAME_None, FMath::Square(CurrentAmount) / 100. + 1, true);
    BarierIdleAnimation->SetRelativeScale3D({ mult, mult, mult });
}

void AMainCell::MoveToCursor()
{
    if (!GEngine) return;
    if (!CameraComponent) return;
    if (!MovementComponent) return;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!PlayerController) return;

    FVector WorldLocation, WorldDirection;
    if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
    {
        // Determine the target location in the world based on the cursor direction
        FVector PawnLocation = GetActorLocation();
        FVector TargetLocation = PawnLocation + (WorldDirection * 1000.0f); // 1000.0f is arbitrary; adjust as needed

        // Calculate direction to move
        FVector Direction = (TargetLocation - PawnLocation).GetSafeNormal();

        // Move the pawn in the desired direction
        AddMovementInput(Direction, MovementSpeed * GetWorld()->GetDeltaSeconds());
    }
}
