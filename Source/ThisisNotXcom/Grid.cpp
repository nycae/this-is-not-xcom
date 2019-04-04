// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"


AGrid::AGrid() : Super()
{
}

void AGrid::AddAtPosition(const FPosition& Position, const ETileState& State)
{
	Tiles.Add(Position, NewObject<ATile>());
	Tiles[Position]->State = State;
}

void AGrid::AddAtCoordinates(const int32& Row, const int32& Column, const ETileState& State)
{
	return AddAtPosition(FPosition(Row, Column), State);
}

bool AGrid::isEmpty(const FPosition& Position) const
{
	if (Tiles.Contains(Position))
		if (Tiles[Position]->State == ETileState::TS_Empty)
			return true;

	return false;
}

bool AGrid::isObstructed(const FPosition& Position) const
{
	if (Tiles.Contains(Position))
		if (Tiles[Position]->State == ETileState::TS_Obstructed)
			return true;
	

	return false;
}

TArray<FPosition> AGrid::GetObstructedPositions() const
{
	TArray<FPosition> ObstructedTiles;

	for (const auto& Pair : Tiles)
		if (Pair.Value->State == ETileState::TS_Obstructed)
			ObstructedTiles.Add(Pair.Key);

	return ObstructedTiles;
}

void AGrid::BeginPlay()
{
	AActor::BeginPlay();

}

void AGrid::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

void AGrid::SetSpawnPoint(const FPosition& Position)
{
	if (Tiles.Contains(Position))
		Tiles[Position]->State = ETileState::TS_Occupied;
}

void AGrid::FreeCoordinate(const FPosition& Position)
{
	if (Tiles.Contains(Position))
		Tiles[Position]->State = ETileState::TS_Empty;
}

void AGrid::OccupiedBy(const FPosition& Position, ATrooper* Occupant)
{
	if (Tiles.Contains(Position)) {
		Tiles[Position]->State = ETileState::TS_Occupied;
		Tiles[Position]->Ocupant = Occupant;
	}
}