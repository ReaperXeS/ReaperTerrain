// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RxGameInstance.generated.h"

class URxSaveGame;
/**
 * 
 */
UCLASS()
class REAPERTERRAIN_API URxGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	virtual void Init() override;

	void ApplySaveGameToWorld(const URxSaveGame* SaveGame) const;

	// Loaded Save Game, kept in memory for the duration of the game instance
	UPROPERTY()
	URxSaveGame* LoadedSaveGame;

	// Const for Save Game Slot Name
	const FString SaveGameSlotName = "SaveGame";
public:
	UFUNCTION(BlueprintCallable)
	void AddPersistedActor(AActor* Actor);

	// Method to remove an actor from the world and add it to the save game
	UFUNCTION(BlueprintCallable)
	void RemovePersistedActor(AActor* Actor);
};
