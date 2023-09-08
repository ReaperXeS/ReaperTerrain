// Fill out your copyright notice in the Description page of Project Settings.


#include "RxLevel.h"

#include "LandscapeComponent.h"
#include "LandscapeEditor/Public/LandscapeEditorObject.h"

// Sets default values
ARxLevel::ARxLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// LandscapeComponent = CreateDefaultSubobject<ULandscapeComponent>(TEXT("LandscapeComponent"));

	LandscapeEditorObject = NewObject<ULandscapeEditorObject>(GetTransientPackage(), TEXT("LandscapeEditorObject"), RF_Transactional);
	// LandscapeEditorObject->SetParent(this);
}

// Called when the game starts or when spawned
void ARxLevel::BeginPlay()
{
	Super::BeginPlay();
	// LandscapeComponent->SetRelativeLocation(FVector(0, 0, 0));
	// LandscapeComponent->SetRelativeRotation(FRotator(0, 0, 0));
	// LandscapeComponent->SetRelativeScale3D(FVector(1, 1, 1));
	// Load HeightMap from file
	// UTexture2D *Heightmap = LoadObject<UTexture2D>(this, TEXT("MaginLandHeightMap.r16"), TEXT("/Game/Heightmap"));

	// See void FEdModeLandscape::ImportHeightData
	// LandscapeComponent->Import(FGuid::NewGuid(), 0, 0, 0, 1, 1, Heightmap, 0, false);
	ImportLandscape();	

	// Load Material from file
	// UMaterial *Material = LoadObject<UMaterial>(this, TEXT("/Game/Materials/Material"));
	// LandscapeComponent->SetMaterial(0, Material);
}

// Taken from LandscapeEditorObject->RefreshImports();
void ARxLevel::ImportLandscape()
{
	// See https://forums.unrealengine.com/t/creating-a-lanscape-using-c-or-the-python-api/504997/4
	// ClearImportLandscapeData();
	// HeightmapImportDescriptorIndex = 0;
	uint32 ImportLandscape_Width = 0;
	uint32 ImportLandscape_Height = 0;
	FLandscapeImportDescriptor HeightmapImportDescriptor;
	
	FText ImportErrorMessage = FText();
	ELandscapeImportResult ImportResult = ELandscapeImportResult::Error;

	if (!ImportLandscape_HeightmapFilename.IsEmpty())
	{		
		ImportResult = FLandscapeImportHelper::GetHeightmapImportDescriptor(ImportLandscape_HeightmapFilename, true, false, HeightmapImportDescriptor, ImportErrorMessage);
		if (ImportResult != ELandscapeImportResult::Error)
		{
			TArray<uint16> ImportLandscape_Data;
			int32 NewLandscape_QuadsPerSection;
			int32 NewLandscape_SectionsPerComponent;
			FIntPoint NewLandscape_ComponentCount;
			constexpr int32 HeightmapImportDescriptorIndex = 0;
			
			ImportLandscape_Width = HeightmapImportDescriptor.ImportResolutions[HeightmapImportDescriptorIndex].Width;
			ImportLandscape_Height = HeightmapImportDescriptor.ImportResolutions[HeightmapImportDescriptorIndex].Height;
			FLandscapeImportHelper::ChooseBestComponentSizeForImport(ImportLandscape_Width, ImportLandscape_Height, NewLandscape_QuadsPerSection, NewLandscape_SectionsPerComponent, NewLandscape_ComponentCount);
			// ImportLandscapeData
			ImportResult = FLandscapeImportHelper::GetHeightmapImportData(HeightmapImportDescriptor, HeightmapImportDescriptorIndex, ImportLandscape_Data, ImportErrorMessage);
			if (ImportResult == ELandscapeImportResult::Error)
			{
				ImportLandscape_Data.Empty();
			} else
			{
				UE_LOG(LogTemp, Error, TEXT("ImportLandscape: %s"), *ImportErrorMessage.ToString());
			}
		} else
		{
			UE_LOG(LogTemp, Error, TEXT("ImportLandscape: %s"), *ImportErrorMessage.ToString());
		}
	}

	// InternalCreateLandscape(HeightmapImportDescriptor.ImportTransform, 63, 1, 1, 1);
	

	// RefreshLayerImports();
	TArray<FLandscapeImportLayer> ImportLandscape_Layers;
	for (FLandscapeImportLayer& UIImportLayer : ImportLandscape_Layers)
	{
		RefreshLayerImport(UIImportLayer, ImportLandscape_Height, ImportLandscape_Width, HeightmapImportDescriptor, 0);
	}
}

void ARxLevel::CreateLandscape()
{
	// See https://forums.unrealengine.com/t/creating-a-lanscape-using-c-or-the-python-api/504997/4
	// ClearImportLandscapeData();
	// HeightmapImportDescriptorIndex = 0;
	uint32 ImportLandscape_Width = 0;
	uint32 ImportLandscape_Height = 0;
	FLandscapeImportDescriptor HeightmapImportDescriptor;
	
	FText ImportErrorMessage = FText();
	ELandscapeImportResult ImportResult = ELandscapeImportResult::Error;

	if (!ImportLandscape_HeightmapFilename.IsEmpty())
	{		
		ImportResult = FLandscapeImportHelper::GetHeightmapImportDescriptor(ImportLandscape_HeightmapFilename, true, false, HeightmapImportDescriptor, ImportErrorMessage);
		if (ImportResult != ELandscapeImportResult::Error)
		{
			TArray<uint16> ImportLandscape_Data;
			int32 NewLandscape_QuadsPerSection;
			int32 NewLandscape_SectionsPerComponent;
			FIntPoint NewLandscape_ComponentCount;
			constexpr int32 HeightmapImportDescriptorIndex = 0;
			
			ImportLandscape_Width = HeightmapImportDescriptor.ImportResolutions[HeightmapImportDescriptorIndex].Width;
			ImportLandscape_Height = HeightmapImportDescriptor.ImportResolutions[HeightmapImportDescriptorIndex].Height;
			FLandscapeImportHelper::ChooseBestComponentSizeForImport(ImportLandscape_Width, ImportLandscape_Height, NewLandscape_QuadsPerSection, NewLandscape_SectionsPerComponent, NewLandscape_ComponentCount);
			// ImportLandscapeData
			ImportResult = FLandscapeImportHelper::GetHeightmapImportData(HeightmapImportDescriptor, HeightmapImportDescriptorIndex, ImportLandscape_Data, ImportErrorMessage);
			if (ImportResult == ELandscapeImportResult::Error)
			{
				ImportLandscape_Data.Empty();
				UE_LOG(LogTemp, Error, TEXT("ImportLandscape: %s"), *ImportErrorMessage.ToString());
				LandscapeRef = nullptr;
			} else
			{
				// Create Landscape
				LandscapeRef = InternalCreateLandscape(FTransform(FVector(0, 0, 0)), NewLandscape_QuadsPerSection, NewLandscape_SectionsPerComponent, NewLandscape_ComponentCount.X, NewLandscape_ComponentCount.Y, ImportLandscape_Data);
			}
		} else
		{
			UE_LOG(LogTemp, Error, TEXT("ImportLandscape: %s"), *ImportErrorMessage.ToString());
		}
	}
}

// TODO: Review with FLandscapeEditorDetailCustomization_NewLandscape::OnCreateButtonClicked
ALandscape* ARxLevel::InternalCreateLandscape(const FTransform& LandscapeTransform, const int32& SectionSize, const int32& SectionsPerComponent, const int32& ComponentCountX, const int32& ComponentCountY, const TArray<uint16> ImportLandscape_Data)
{
	int32 QuadsPerComponent = SectionSize * SectionsPerComponent;

	int32 SizeX = ComponentCountX * QuadsPerComponent + 1;
	int32 SizeY = ComponentCountY * QuadsPerComponent + 1;

	TArray<FLandscapeImportLayerInfo> MaterialImportLayers;
	// MaterialImportLayers.Reserve(0);


	TMap<FGuid, TArray<uint16>> HeightDataPerLayers;
	TMap<FGuid, TArray<FLandscapeImportLayerInfo>> MaterialLayerDataPerLayers;


	TArray<uint16> HeightData;
	HeightData.SetNum(SizeX * SizeY);
	for (int32 i = 0; i < HeightData.Num(); i++)
	{
		HeightData[i] = 32768;
	}

	// HeightDataPerLayers.Add(FGuid(), MoveTemp(HeightData)); /*ENewLandscapePreviewMode.NewLandscape*/
	HeightDataPerLayers.Add(FGuid(), ImportLandscape_Data); /*ENewLandscapePreviewMode.NewLandscape*/
	// ComputeHeightData will also modify/expand material layers data, which is why we create MaterialLayerDataPerLayers after calling ComputeHeightData

	FLandscapeImportLayerInfo LayInfoGrass;
	LayInfoGrass.LayerData = LandscapeLayerInfoAuto.LayerData;
	// LayInfoGrass.LayerData.AddUninitialized(TotalSize);
	LayInfoGrass.LayerName = "Auto";
	// LayInfoGrass.SourceFilePath = "path/to/weightmap.png";
	// Important to have a layer info, without it, it will not show at all, and crash.
	LayInfoGrass.LayerInfo = LandscapeLayerInfoAuto.LayerInfo; 
	// ReadWeightmapFile(LayInfoGrass.LayerData, LayInfoGrass.SourceFilePath, LayInfoGrass.LayerName, SizeX, SizeY);
	MaterialImportLayers.Add(LayInfoGrass);
	
	// MaterialImportLayers.Add(LandscapeLayerInfoAuto);
	
	MaterialLayerDataPerLayers.Add(FGuid(), MaterialImportLayers);
	

	//FScopedTransaction Transaction(TEXT("Undo", "Creating New Landscape"));

	UWorld* World = nullptr;
	{
		// We want to create the landscape in the landscape editor mode's world
		FWorldContext& EditorWorldContext = GEditor->GetEditorWorldContext();
		World = EditorWorldContext.World();
	}

	ALandscape* Landscape = World->SpawnActor<ALandscape>();
	Landscape->bCanHaveLayersContent = true;
	Landscape->LandscapeMaterial = LandscapeMaterial;

	Landscape->SetActorTransform(LandscapeTransform);

	// automatically calculate a lighting LOD that won't crash lightmass (hopefully)
	// < 2048x2048 -> LOD0
	// >=2048x2048 -> LOD1
	// >= 4096x4096 -> LOD2
	// >= 8192x8192 -> LOD3


	//const FGuid& InGuid, int32 InMinX, int32 InMinY, int32 InMaxX, int32 InMaxY, int32 InNumSubsections, int32 InSubsectionSizeQuads, const TMap<FGuid, TArray<uint16>>& InImportHeightData,
	//	const TCHAR* const InHeightmapFileName, const TMap<FGuid, TArray<FLandscapeImportLayerInfo>>& InImportMaterialLayerInfos, ELandscapeImportAlphamapType InImportMaterialLayerType, const TArray<struct FLandscapeLayer>* InImportLayers = nullptr
	// const TArray<struct FLandscapeLayer>* InImportLayers;
	Landscape->Import(FGuid::NewGuid(), 0, 0, SizeX - 1, SizeY - 1, SectionsPerComponent, SectionSize, HeightDataPerLayers, nullptr, MaterialLayerDataPerLayers, ELandscapeImportAlphamapType::Additive);

	Landscape->StaticLightingLOD = FMath::DivideAndRoundUp(FMath::CeilLogTwo((SizeX * SizeY) / (2048 * 2048) + 1), (uint32)2);
	// Register all the landscape components
	ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();
	LandscapeInfo->Layers[0].LayerInfoObj = LandscapeLayerInfoAuto.LayerInfo;

	LandscapeInfo->UpdateLayerInfoMap(Landscape);
	LandscapeInfo->RecreateCollisionComponents();


	Landscape->RegisterAllComponents();
	Landscape->RecreateCollisionComponents();

	// Need to explicitly call PostEditChange on the LandscapeMaterial property or the landscape proxy won't update its material
	FPropertyChangedEvent MaterialPropertyChangedEvent(FindFieldChecked< FProperty >(Landscape->GetClass(), FName("LandscapeMaterial")));
	Landscape->PostEditChangeProperty(MaterialPropertyChangedEvent);
	Landscape->PostEditChange();
	
	return Landscape;
}

// Taken from LandscapeEditorObject->RefreshLayerImport();
void ARxLevel::RefreshLayerImport(FLandscapeImportLayer& ImportLayer, uint32 LandscapeHeight, uint32 LandscapeWidth, FLandscapeImportDescriptor HeightmapImportDescriptor, int32 HeightmapImportDescriptorIndex)
{
	ImportLayer.ErrorMessage = FText();
	ImportLayer.ImportResult = ELandscapeImportResult::Success;

	if (ImportLayer.LayerName == ALandscapeProxy::VisibilityLayer->LayerName)
	{
		ImportLayer.LayerInfo = ALandscapeProxy::VisibilityLayer;
	}

	if (!ImportLayer.SourceFilePath.IsEmpty())
	{
		if (!ImportLayer.LayerInfo)
		{
			ImportLayer.ImportResult = ELandscapeImportResult::Error;
			ImportLayer.ErrorMessage = NSLOCTEXT("LandscapeEditor.NewLandscape", "Import_LayerInfoNotSet", "Can't import a layer file without a layer info");
		}
		else
		{
			ImportLayer.ImportResult = FLandscapeImportHelper::GetWeightmapImportDescriptor(ImportLayer.SourceFilePath, true, false, ImportLayer.LayerName, ImportLayer.ImportDescriptor, ImportLayer.ErrorMessage);
			if (ImportLayer.ImportResult != ELandscapeImportResult::Error)
			{
				if (LandscapeHeight != 0 || LandscapeWidth != 0)
				{
					// Use same import index as Heightmap
					int32 FoundIndex = INDEX_NONE;
					for (int32 Index = 0; Index < ImportLayer.ImportDescriptor.FileResolutions.Num(); ++Index)
					{
						if (ImportLayer.ImportDescriptor.ImportResolutions[Index] == HeightmapImportDescriptor.ImportResolutions[HeightmapImportDescriptorIndex])
						{
							FoundIndex = Index;
							break;
						}
					}

					if (FoundIndex == INDEX_NONE)
					{
						ImportLayer.ImportResult = ELandscapeImportResult::Error;
						ImportLayer.ErrorMessage = NSLOCTEXT("LandscapeEditor.ImportLandscape", "Import_WeightHeightResolutionMismatch", "Weightmap import resolution isn't same as Heightmap resolution.");
					}
				}
			}
		}
	}
}

