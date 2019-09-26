// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "Grid.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

UCLASS()
class THISISNOTXCOM_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void GenerateGround();

	void GenerateBlockingPattern();

	void FixPattern();

	void SpawnBlockingMeshes();

	ATile* SpawnFloor(const FVector& Position);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Elements")
		AGrid* CombatGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Size")
		int32 SideSizeX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Size")
		int32 SideSizeY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Size")
		FVector StartingPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Tiles")
		TArray<FPosition> AditionalTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Tiles")
		TArray<FPosition> SpawnPositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Characteristics")
		float BlockChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structures")
		TSubclassOf<AActor> FloorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structures")
		UInstancedStaticMeshComponent* ObstructingStructure;

};
