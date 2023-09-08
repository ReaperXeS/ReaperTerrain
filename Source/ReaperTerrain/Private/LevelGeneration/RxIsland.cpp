// Fill out your copyright notice in the Description page of Project Settings.
#include "ReaperTerrain/Public/LevelGeneration/RxIsland.h"
#include "Components/BoxComponent.h"
#include "Engine/LevelStreamingDynamic.h"

// Sets default values
ARxIsland::ARxIsland()
{
	PrimaryActorTick.bCanEverTick = false;

	Outline = CreateDefaultSubobject<UBoxComponent>(TEXT("Outline"));
	Outline->SetBoxExtent(FVector(Size.X, Size.Y, 0.f));
	Outline->ShapeColor = FColor::Green;
	Outline->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Outline->SetHiddenInGame(true);
	Outline->SetLineThickness(1000.f);
	SetRootComponent(Outline);
}

// Called when the game starts or when spawned
void ARxIsland::BeginPlay()
{
	Super::BeginPlay();
	LoadLevel();
}

void ARxIsland::LoadLevel()
{
	if (LoadedLevel)
	{
		LoadedLevel->SetShouldBeVisible(false);
		LoadedLevel->SetIsRequestingUnloadAndRemoval(true);
		LoadedLevel->SetShouldBeLoaded(false);
		GetWorld()->RemoveStreamingLevel(LoadedLevel);
		LoadedLevel = nullptr;
	}
	
	// Load Level instance
	FVector LevelPosition = FVector(GetActorLocation().X - Size.X / 2, GetActorLocation().Y - Size.Y /2, GetActorLocation().Z);
	bool bOutSuccess = false;
	LoadedLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), LevelToLoad, LevelPosition, GetActorRotation(), bOutSuccess);
	UE_LOG(LogTemp, Warning, TEXT("LoadLevel success: %d"), bOutSuccess);
}

void ARxIsland::SetSize(const FVector2D& InSize)
{
	Size = InSize;
	Outline->SetBoxExtent(FVector(Size.X/2, Size.Y/2, 0.f));
}
