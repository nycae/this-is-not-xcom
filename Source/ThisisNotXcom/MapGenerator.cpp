// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"
#include "AStar.h"
#include "Engine.h"

// Sets default values
AMapGenerator::AMapGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMapGenerator::GenerateBlockingPattern()
{
	for (int I = 0; I < SideSizeX - 1; I++) 
	{
		for (int J = 0; J < SideSizeY - 1 - I; J++) 
		{
			if (BlockChance > FMath::FRandRange(0.0, 1.0))
			{
				CombatGrid->ObstructPosition(FPosition(I, J));
				CombatGrid->ObstructPosition(FPosition(((SideSizeX - 1) - J), ((SideSizeY - 1) - I)));
			}
		}
	}
}

void AMapGenerator::FixPattern()
{
	for (int32 i = 2; i < FMath::Min(SideSizeX, SideSizeY); i++) 
	{
		CombatGrid->FreeCoordinate(FPosition(i, i));

		CombatGrid->FreeCoordinate(FPosition(i + 1, i));
		CombatGrid->FreeCoordinate(FPosition(i - 1, i));

		CombatGrid->FreeCoordinate(FPosition(i, i + 1));
		CombatGrid->FreeCoordinate(FPosition(i, i - 1));
	}
}

void AMapGenerator::SpawnBlockingMeshes()
{
	for (const auto& BlockedTile : CombatGrid->GetObstructedPositions())
	{
		ObstructingStructure->AddInstance(FTransform
			(FVector(BlockedTile.Row * CombatGrid->TileSize, BlockedTile.Column * CombatGrid->TileSize, -10.0) + StartingPosition));
	}
}

void AMapGenerator::GenerateGround()
{
	for (int I = 0; I < SideSizeX; I++)
	{
		for (int J = 0; J < SideSizeY; J++)
		{
			CombatGrid->AddAtCoordinates(I, J, SpawnFloor
				(FVector(I * CombatGrid->TileSize, J * CombatGrid->TileSize, -60.0) + StartingPosition));
		}
	}

	for (const auto& Position : AditionalTiles)
	{
		if( ((Position.Row < 0) && (Position.Column < 0))
			||
			((Position.Row >= SideSizeX) && (Position.Column >= SideSizeY)) )
		{
			CombatGrid->AddAtPosition(Position, SpawnFloor
				(FVector(Position.Row * CombatGrid->TileSize, Position.Column * CombatGrid->TileSize, -60.0) + StartingPosition));
		}
	}
}

void AMapGenerator::GenerateFinalGround()
{
	GenerateBlockingPattern();

	if (!AStar(CombatGrid, SideSizeX, SideSizeY).isPossiblePathExisting(FPosition(0, 0)))
	{
		FixPattern();
	}

	SpawnBlockingMeshes();
}

void AMapGenerator::BeginPlay()
{
	AActor::BeginPlay();

	GenerateGround();

}

ATile* AMapGenerator::SpawnFloor(const FVector& Position)
{
	return (ATile*) GetWorld()->SpawnActor(FloorClass, &Position);
}