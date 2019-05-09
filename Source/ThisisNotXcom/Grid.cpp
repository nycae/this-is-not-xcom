// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "Engine.h"
#include "GridPathfinder.h"

AGrid::AGrid() : Super()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AGrid::AddAtPosition(const FPosition& Position, const ETileState& State, ATile* Tile)
{
	if (Tile)
	{
		Tiles.Add(Position, Tile);
	}
	else
	{
		Tiles.Add(Position, NewObject<ATile>());
	}

	Tiles[Position]->State = State;

}

void AGrid::AddAtCoordinates(const int32& Row, const int32& Column, const ETileState& State, ATile* Tile)
{
	return AddAtPosition(FPosition(Row, Column), State, Tile);
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
	{
		Tiles[Position]->State = ETileState::TS_Empty;
		Tiles[Position]->Ocupant = nullptr;
	}
}

void AGrid::OccupiedBy(const FPosition& Position, ATrooper* Occupant)
{
	if (Tiles.Contains(Position)) 
	{
		Tiles[Position]->State = ETileState::TS_Occupied;
		Tiles[Position]->Ocupant = Occupant;
	}
}

TArray<EDirectionEnum> AGrid::GetPath(const FPosition& Origin, const FPosition& Destiny, uint8 MaxDepth) const
{
	auto Cosa = GridPathfinder::GetPath(Origin, Destiny, MaxDepth, this);
	GridPathfinder::PrintPath(Cosa);
	return Cosa;
}

void AGrid::AddOcupantAt(ATrooper* NewOcupant, const FPosition& Position)
{
	if (Tiles.Contains(Position))
	{
		Tiles[Position]->Ocupant = NewOcupant;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Esa posición no existe");
	}

}

FPosition AGrid::GetPosition(ATile* Tile) const
{
	return *Tiles.FindKey(Tile);
}

void AGrid::SwapPositions(const FPosition& Origin, const FPosition& Destiny)
{
	//Swap(Tiles[Origin], Tiles[Destiny]); // Doesn't work
	auto LazyBuffer = Tiles[Origin]->Ocupant;
	Tiles[Origin]->Ocupant = Tiles[Destiny]->Ocupant;
	Tiles[Destiny]->Ocupant = LazyBuffer;

	Tiles[Origin]->State = ETileState::TS_Empty;
	Tiles[Destiny]->State = ETileState::TS_Occupied;

}