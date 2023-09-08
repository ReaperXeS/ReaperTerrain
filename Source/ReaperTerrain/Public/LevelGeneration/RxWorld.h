// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RxWorld.generated.h"

class UBoxComponent;
class ARxIsland;

UCLASS()
class REAPERTERRAIN_API ARxWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARxWorld();
	// Destroy
	virtual void Destroyed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// World size in centimeters
	UPROPERTY(EditAnywhere, Category = "RxWorld")
	FVector2D WorldSize = FVector2D(500000, 500000);

	// Grid section count in X and Y
	UPROPERTY(EditAnywhere, Category = "RxWorld")
	FVector2D GridSectionCount = FVector2D(5,5);

	UPROPERTY(VisibleAnywhere, Category = "RxWorld")
	TArray<TObjectPtr<ARxIsland>> Islands;

	UPROPERTY(EditAnywhere, Category = "RxWorld")
	TArray<TSoftObjectPtr<UWorld>> Levels;

	// Island Class
	UPROPERTY(EditAnywhere, Category = "RxWorld")
	TSubclassOf<ARxIsland> IslandClass;

	UPROPERTY(EditAnywhere, Category = "RxWorld")
	TSubclassOf<AActor> Ocean;

	UPROPERTY(EditAnywhere, Category = "RxWorld")
	TSubclassOf<AActor> WeatherSystem;

	UPROPERTY(EditAnywhere, Category = "RxWorld")
	float Seed = 0.0f;

	// Validate properties
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void ClearIslands();
	void GenerateIslands();

	UPROPERTY(VisibleAnywhere, Category = "RxWorld")
	UBoxComponent* Outline;

	virtual void OnConstruction(const FTransform& Transform) override;
	
};
