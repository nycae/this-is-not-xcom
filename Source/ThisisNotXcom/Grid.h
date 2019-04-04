// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Position.h"
#include "Tile.h"
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

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
		void AddAtCoordinates(const int32& Row, const int32& Column, const ETileState& State);

	UFUNCTION(BlueprintCallable)
		void AddAtPosition(const FPosition& Position, const ETileState& State);

	UFUNCTION(BlueprintCallable)
		bool isEmpty(const FPosition& Position) const;

	UFUNCTION(BlueprintCallable)
		bool isObstructed(const FPosition& Position) const;

	UFUNCTION(BlueprintCallable)
		TArray<FPosition> GetObstructedPositions() const;

	UFUNCTION(BlueprintCallable)
		void SetSpawnPoint(const FPosition& Position);

	UFUNCTION(BlueprintCallable)
		void FreeCoordinate(const FPosition& Position);

	UFUNCTION(BlueprintCallable)
		void OccupiedBy(const FPosition& Position, ATrooper* Occupant);

private:

	TMap<FPosition, ATile*> Tiles;

};
