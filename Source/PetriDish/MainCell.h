// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h" 
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"    
#include "MainCell.generated.h"

UCLASS()
class PETRIDISH_API AMainCell : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainCell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void RorateRight(const FInputActionValue& Value);

	void PlayMoveAnimation(bool isMoving);

private:
	void MoveToCursor();

	UPROPERTY(EditAnywhere, Category = "Desh move")
	float MovementSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Desh move")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = "Desh move")
	class UFloatingPawnMovement* MovementComponent;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* _inputMappingContext;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* _goForwardAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* _goRightAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* _rotateAction;

	// Force multiplier for movement
	UPROPERTY(EditAnywhere, Category = "WASD Movement")
	float MovementForce;

	// Torque multiplier for rotation
	UPROPERTY(EditAnywhere, Category = "WASD Movement")
	float RotationTorque;

	// Torque multiplier for rotation
	UPROPERTY(EditAnywhere, Category = "WASD Movement")
	float FrictionForce;

	// Torque multiplier for rotation
	UPROPERTY(EditAnywhere, Category = "WASD Movement")
	float MinSpeed;

	// Torque multiplier for rotation
	UPROPERTY(EditAnywhere, Category = "WASD Movement")
	float RotateFrictionMult;

	// Torque multiplier for rotation
	UPROPERTY(EditAnywhere, Category = "WASD Movement")
	float MinRotationSpeed;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UPaperFlipbookComponent* EdroIdleAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UPaperFlipbookComponent* BarierIdleAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UPaperFlipbook* EdroIdleAnimationClip;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UPaperFlipbook* BarierIdleAnimationClip;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollider;
};
