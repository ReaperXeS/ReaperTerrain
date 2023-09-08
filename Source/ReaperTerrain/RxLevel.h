// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandscapeProxy.h"
#include "RxLevel.generated.h"

class ALandscape;
struct FLandscapeImportDescriptor;
struct FLandscapeImportLayer;
class ULandscapeEditorObject;
class ULandscapeComponent;

UCLASS()
class REAPERTERRAIN_API ARxLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARxLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Landscape")
	ULandscapeComponent* LandscapeComponent;

	UPROPERTY()
	ULandscapeEditorObject* LandscapeEditorObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Landscape")
	FString ImportLandscape_HeightmapFilename;

	void ImportLandscape();

	UFUNCTION(CallInEditor, Category = "Landscape")
	void CreateLandscape();

	ALandscape* InternalCreateLandscape(const FTransform& LandscapeTransform, const int32& SectionSize, const int32& SectionsPerComponent, const int32& ComponentCountX, const int32& ComponentCountY, const TArray<uint16> ImportLandscape_Data);

	UPROPERTY()
	ALandscape* LandscapeRef;

	/** Combined material used to render the landscape */
	UPROPERTY(EditAnywhere, Category = "Landscape")
	TObjectPtr<UMaterialInterface> LandscapeMaterial;

	UPROPERTY(EditAnywhere, Category = "Landscape")
	FLandscapeImportLayerInfo LandscapeLayerInfoAuto;

	void RefreshLayerImport(FLandscapeImportLayer& ImportLayer, uint32 LandscapeHeight, uint32 LandscapeWidth, FLandscapeImportDescriptor HeightmapImportDescriptor, int32 HeightmapImportDescriptorIndex);
public:	

};
