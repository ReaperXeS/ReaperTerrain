// Fill out your copyright notice in the Description page of Project Settings.
#include "ReaperTerrain/Public/LevelGeneration/RxWorld.h"
#include "ReaperTerrain/Public/LevelGeneration/RxIsland.h"
#include "Components/BoxComponent.h"

// Sets default values
ARxWorld::ARxWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Outline = CreateDefaultSubobject<UBoxComponent>(TEXT("Outline"));
	Outline->SetBoxExtent(FVector(WorldSize.X/2, WorldSize.Y/2, 0.f));
	Outline->ShapeColor = FColor::Blue;
	Outline->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Outline->SetHiddenInGame(false);
	Outline->SetLineThickness(5.f);
	SetRootComponent(Outline);
}

void ARxWorld::Destroyed()
{
	ClearIslands();
	
	Super::Destroyed();
}

// Called when the game starts or when spawned
void ARxWorld::BeginPlay()
{
	Super::BeginPlay();
}

void ARxWorld::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ARxWorld, WorldSize))
	{
		WorldSize.X = FMath::Max(WorldSize.X, 0.0f);
		WorldSize.Y = FMath::Max(WorldSize.Y, 0.0f);
	}
	else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ARxWorld, GridSectionCount))
	{
		GridSectionCount.X = FMath::Max(GridSectionCount.X, 1.0f);
		GridSectionCount.Y = FMath::Max(GridSectionCount.Y, 1.0f);
	}
	else if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ARxWorld, Seed))
	{
		Seed = FMath::Max(Seed, 0.0f);
		FMath::RandInit(Seed);
	}
}

void ARxWorld::ClearIslands()
{
	// Clear islands
	for (ARxIsland* Island : Islands)
	{
		if (Island != nullptr)
		{
			Island->Destroy();
		}
	}
	Islands.Empty();
}

void ARxWorld::GenerateIslands()
{
	FMath::RandInit(Seed); // Always spawn islands in same order
	ClearIslands();

	for (int i = 0; i < 10; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Random: %d"), FMath::RandRange(0, 100));
	}

	if (IslandClass != nullptr)
	{
		// Compute Grid Section sizes
		const float XSize = WorldSize.X / GridSectionCount.X;
		const float YSize = WorldSize.Y / GridSectionCount.Y;
		
		// Create Grid Sections with World Size & Grid Section
		for (float X = -WorldSize.X / 2 + XSize / 2; X < WorldSize.X/2; X += XSize)
		{
			for (float Y = -WorldSize.Y/2 + YSize / 2; Y < WorldSize.Y/2; Y += YSize)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				
				ARxIsland* Island =  GetWorld()->SpawnActor<ARxIsland>(IslandClass, FTransform(), SpawnParams);
				Island->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				Island->SetSize(FVector2D(XSize, YSize));
				Island->SetActorRelativeLocation(FVector(X, Y, 0.f));
				if (Levels.Num() > 0)
				{
					Island->SetLevelToLoad(Levels[FMath::RandRange(0, Levels.Num()-1)]);
				}
				Islands.Add(Island);
			}
		}
	}
}

void ARxWorld::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GenerateIslands();
	Outline->SetBoxExtent(FVector(WorldSize.X/2, WorldSize.Y/2, 0.f));
}

