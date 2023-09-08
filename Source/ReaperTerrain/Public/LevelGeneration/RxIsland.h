// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RxIsland.generated.h"

class ULevelStreamingDynamic;
class UBoxComponent;

UCLASS()
class REAPERTERRAIN_API ARxIsland : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARxIsland();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "RxIsland")
	UBoxComponent* Outline;

	UPROPERTY(EditAnywhere, Category = "RxIsland")
	TSoftObjectPtr<UWorld> LevelToLoad;

	UPROPERTY(VisibleAnywhere, Category = "RxIsland")
	FVector2D Size;

	void LoadLevel();

	UPROPERTY()
	ULevelStreamingDynamic* LoadedLevel;
public:
	FORCEINLINE FVector2D GetSize() const { return Size; }
	void SetSize(const FVector2D& InSize);
	void SetLevelToLoad(const TSoftObjectPtr<UWorld>& InLevelToLoad) { LevelToLoad = InLevelToLoad; }	
};
