// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "Engine.h"
#include "GridPathfinder.h"

AGrid::AGrid() : Super()
{
	PrimaryActorTick.bCanEverTick = false;

}

ATile* AGrid::At(const FPosition& Position)
{
	return Tiles[Position];
}

void AGrid::AddAtPosition(const FPosition& Position, ATile* Tile)
{
	if (Tile)
	{
		Tiles.Add(Position, Tile);
	}
	else
	{
		Tiles.Add(Position, NewObject<ATile>());
	}

}

void AGrid::AddAtCoordinates(int32 Row, int32 Column, ATile* Tile)

{
	return AddAtPosition(FPosition(Row, Column), Tile);
}

bool AGrid::isEmpty(const FPosition& Position) const
{
	if (Tiles.Contains(Position))
	{
		if (Tiles[Position]->State == ETileState::TS_Empty)
		{
			return true;
		}
	}

	return false;
}

bool AGrid::isObstructed(const FPosition& Position) const
{
	if (Tiles.Contains(Position))
	{
		if (Tiles[Position]->State == ETileState::TS_Obstructed)
		{
			return true;
		}
	}

	return false;
}

TArray<FPosition> AGrid::GetObstructedPositions() const
{
	TArray<FPosition> ObstructedTiles;

	for (const auto& Pair : Tiles)
	{
		if (Pair.Value->State == ETileState::TS_Obstructed)
		{
			ObstructedTiles.Add(Pair.Key);
		}
	}

	return ObstructedTiles;
}

TArray<FPosition> AGrid::GetOccupiedPositions() const
{
	TArray<FPosition> OccupiedTiles;

	for (const auto& Pair : Tiles)
	{
		if (Pair.Value->State == ETileState::TS_Occupied)
		{
			OccupiedTiles.Add(Pair.Key);
		}
	}

	return OccupiedTiles;
}

void AGrid::SetSpawnPoint(const FPosition& Position)
{
	if (Tiles.Contains(Position))
	{
		Tiles[Position]->State = ETileState::TS_Occupied;
	}
}

void AGrid::FreeCoordinate(const FPosition& Position)
{
	if (Tiles.Contains(Position))
	{
		Tiles[Position]->State = ETileState::TS_Empty;
		Tiles[Position]->Ocupant = nullptr;
	}
}

void AGrid::OccupiedBy(const FPosition& Position, AUnit* Occupant)
{
	if (Tiles.Contains(Position)) 
	{
		Tiles[Position]->State = ETileState::TS_Occupied;
		Tiles[Position]->Ocupant = Occupant;
	}
}

TArray<EDirectionEnum> AGrid::GetPath(const FPosition& Origin, const FPosition& Destiny, uint8 MaxDepth) const
{
	return GridPathfinder::GetPath(Origin, Destiny, MaxDepth, this);
}

void AGrid::AddOcupantAt(AUnit* NewOcupant, const FPosition& Position)
{
	if (Tiles.Contains(Position))
	{
		Tiles[Position]->Ocupant = NewOcupant;
	}
}

FPosition AGrid::GetPosition(const ATile* Tile) const
{
	for (const auto& Pair : Tiles)
	{
		if (Pair.Value == Tile)
		{
			return Pair.Key;
		}
	}

	return FPosition();
}

void AGrid::ObstructPosition(const FPosition& Position)
{
	if (Tiles.Contains(Position))
	{
		Tiles[Position]->State = ETileState::TS_Obstructed;
	}
}

void AGrid::SwapPositions(const FPosition& Origin, const FPosition& Destiny)
{
	auto LazyBuffer = Tiles[Origin]->Ocupant;
	Tiles[Origin]->Ocupant = Tiles[Destiny]->Ocupant;
	Tiles[Destiny]->Ocupant = LazyBuffer;

	Tiles[Origin]->State = ETileState::TS_Empty;
	Tiles[Destiny]->State = ETileState::TS_Occupied;
}