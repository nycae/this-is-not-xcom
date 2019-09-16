// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Position.h"
#include "Tile.h"
#include "Direction.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

/**
 * 
 */
UCLASS()
class THISISNOTXCOM_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:

	AGrid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
		int32 TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
		TMap<FPosition, ATile*> Tiles;

public:

	UFUNCTION(BlueprintCallable)
		void ObstructPosition(const FPosition& Position);

	UFUNCTION(BlueprintCallable)
		void AddAtCoordinates(int32 Row, int32 Column, ATile* Tile);

	UFUNCTION(BlueprintCallable)
		void AddAtPosition(const FPosition& Position, ATile* Tile);

	UFUNCTION(BlueprintCallable)
		bool isEmpty(const FPosition& Position) const;

	UFUNCTION(BlueprintCallable)
		bool isObstructed(const FPosition& Position) const;

	UFUNCTION(BlueprintCallable)
		TArray<FPosition> GetObstructedPositions() const;

	UFUNCTION(BlueprintCallable)
		FPosition GetPosition(const ATile* Tile) const;

	UFUNCTION(BlueprintCallable)
		void SetSpawnPoint(const FPosition& Position);

	UFUNCTION(BlueprintCallable)
		void FreeCoordinate(const FPosition& Position);

	UFUNCTION(BlueprintCallable)
		void OccupiedBy(const FPosition& Position, AUnit* Occupant);

	UFUNCTION(BlueprintCallable)
		TArray<EDirectionEnum> GetPath(const FPosition& Origin, const FPosition& Destiny, uint8 MaxDepth) const;

	UFUNCTION(BlueprintCallable)
		void AddOcupantAt(AUnit* NewOcupant, const FPosition& Position);

	UFUNCTION(BlueprintCallable)
		void SwapPositions(const FPosition& Origin, const FPosition& Destiny);

};
