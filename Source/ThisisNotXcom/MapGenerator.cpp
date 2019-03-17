// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"
#include "Engine.h"

// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMapGenerator::PreGenerateGround()
{
	// Spawn floor and generate obstructed tiles
	for (int i = 0; i < MapMaxX; i++) {
		for (int j = 0; j < MapMaxY; j++) {
			Floor->AddInstance(FTransform(FVector(float(i * TileSize), float(j * TileSize), -10.0)));
			if (BlockChance > FMath::FRandRange(0.0, 1.0)) {
				CombatGrid->AddAtCoordinates(i, j, ETileState::TS_Obstructed);
			}
			else {
				CombatGrid->AddAtCoordinates(i, j, ETileState::TS_Empty);
			}
		}
	}
	SetPlayerTroops();
}

void AMapGenerator::SetPlayerTroops()
{
	/* Spawn troops */
	// Player one
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			switch (i+j) {
				case 0: case 1: /* Spawn Champion */ 
					// Create Pawn
					CombatGrid->SetSpawnPoint(FPosition(i, j));
					CombatGrid->SetSpawnPoint(FPosition((MapMaxX - 1) - i, (MapMaxY - 1) - j));
					break;
				case 2: case 3: /* Spawn Regular */ 
					// Create Champion
					CombatGrid->SetSpawnPoint(FPosition(i, j));
					CombatGrid->SetSpawnPoint(FPosition((MapMaxX - 1) - i, (MapMaxY - 1) - j));
					break;
				default: 
					break;
			}
		}
	}
}

void AMapGenerator::FixGround()
{
	// Find a path in the grid
}

void AMapGenerator::GenerateGround()
{
	for (const auto& Position : CombatGrid->GetObstructedPositions())
		ObstructingStructure->AddInstance((FTransform(FVector(Position.Row * TileSize, Position.Column * TileSize, -10.0))));
}


void AMapGenerator::FindGrid()
{
	TArray<AActor*> Grids;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), Grids);

	if (Grids.Num() == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No grids were found"));
		return;
	}

	CombatGrid = Cast<AGrid>(Grids[0]);
}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	FindGrid();

	PreGenerateGround();

	FixGround();

	GenerateGround();
}

// Called every frame
void AMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

