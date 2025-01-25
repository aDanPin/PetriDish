// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneManager.h"

// Sets default values
ASceneManager::ASceneManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASceneManager::BeginPlay()
{
	Super::BeginPlay();

    SpawnEnemyes();
    SpawnResources();
}

// Called every frame
void ASceneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASceneManager::SpawnEnemyes() {
    for (int i = 0; i < EnemyNum; i++) {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        //SpawnParams.Instigator = GetInstigator(); // Optional: pass instigator if needed

        FVector SpawnVector(FMath::FRandRange(MinRadius, MaxRadius), 0, 100.);

        FRotator RotationZ(0.0f, FMath::FRandRange(0., 360.), 0.0f); // Yaw = 45 degrees
        FVector SpawnLocation = RotationZ.RotateVector(SpawnVector);
    
        FRotator SpawnRotation(0.0f, FMath::FRandRange(0., 360.), 0.0f); // Yaw = 45 degrees

        // Spawn the Pawn
        APawn* SpawnedPawn = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);
       }
}

void ASceneManager::SpawnResources() {
    for (int i = 0; i < ResourceNum; i++) {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        //SpawnParams.Instigator = GetInstigator(); // Optional: pass instigator if needed

        FVector SpawnVector(FMath::FRandRange(MinRadius, MaxRadius), 0, 100.);

        FRotator RotationZ(0.0f, FMath::FRandRange(0., 360.), 0.0f); // Yaw = 45 degrees
        FVector SpawnLocation = RotationZ.RotateVector(SpawnVector);

        FRotator SpawnRotation(0.0f, FMath::FRandRange(0., 360.), 0.0f); // Yaw = 45 degrees

        // Spawn the Pawn
        APawn* SpawnedPawn = GetWorld()->SpawnActor<AResource>(ResourceClass, SpawnLocation, SpawnRotation, SpawnParams);
    }
}
