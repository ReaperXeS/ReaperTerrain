// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RxGameInstance.h"

#include "Game/RxSaveGame.h"
#include "Kismet/GameplayStatics.h"

void URxGameInstance::Init()
{
	Super::Init();

	// Validate Save Game Exists
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, 0))
	{
		// Yes, Load Save Game and apply to World
		LoadedSaveGame = Cast<URxSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0));
		if (LoadedSaveGame)
		{
			ApplySaveGameToWorld(LoadedSaveGame);
		}
	}

	// Create Save Game if it doesn't exist
	if (LoadedSaveGame == nullptr)
	{		
		LoadedSaveGame = Cast<URxSaveGame>(UGameplayStatics::CreateSaveGameObject(URxSaveGame::StaticClass()));
	}
}

void URxGameInstance::ApplySaveGameToWorld(const URxSaveGame* SaveGame) const
{
	// Keep track of removed actors
	LoadedSaveGame->PersistedActorRemovedMap = SaveGame->PersistedActorRemovedMap;

	// Destroy Actor if it exists in the world
	for (const TPair<FString, TSubclassOf<AActor>> Element : SaveGame->PersistedActorRemovedMap)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(this, Element.Value, FoundActors);
		for (AActor* FoundActor : FoundActors)
		{
			if (FoundActor && FoundActor->GetName() == Element.Key)
			FoundActor->Destroy();
		}		
	}	
}

void URxGameInstance::AddPersistedActor(AActor* Actor)
{
	// TODO: Implement AddPersistedActor function will probably need another map for this.
	// const TSubclassOf<AActor> ActorClass = LoadedSaveGame->PersistedActorRemovedMap.FindKey(Actor->GetName());
}

void URxGameInstance::RemovePersistedActor(AActor* Actor)
{
	LoadedSaveGame->PersistedActorRemovedMap.Add(Actor->GetName(), Actor->GetClass());
	UGameplayStatics::SaveGameToSlot(LoadedSaveGame, SaveGameSlotName, 0);
	Actor->Destroy();
}
