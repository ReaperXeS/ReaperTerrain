// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RxSaveGame.generated.h"

USTRUCT()
struct FPersistedActor
{
	GENERATED_BODY()

	TArray<FString> ActorNames;
};

/**
 * 
 */
UCLASS()
class REAPERTERRAIN_API URxSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TMap<FString, TSubclassOf<AActor>> PersistedActorRemovedMap;
};
