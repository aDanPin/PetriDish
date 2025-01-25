// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "Resource.h"
#include "SceneManager.generated.h"

UCLASS()
class PETRIDISH_API ASceneManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASceneManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float MinRadius;
	UPROPERTY(EditAnywhere, Category = "Spawning")
	float MaxRadius;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int EnemyNum;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int ResourceNum;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AEnemy> EnemyClass;

	//UPROPERTY(EditAnywhere, Category = "Spawning")
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AResource> ResourceClass;

	void SpawnEnemyes();

	void SpawnResources();
};
