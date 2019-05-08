// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"
#include "AStar.h"
#include "Engine.h"

// Sets default values
AMapGenerator::AMapGenerator()
	: TroopManager(NewObject<ATroopManager>())
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AMapGenerator::PreGenerateGround()
{
	FVector SpawnPosition1;
	FVector SpawnPosition2;

	ATile* TileBuffer1;
	ATile* TileBuffer2;

	// Spawn floor and generate obstructed tiles
	for (int i = 0; i < MapMaxX; i++) 
	{
		for (int j = 0; j < MapMaxY - i; j++) 
		{

			SpawnPosition1 = FVector(float(i * TileSize), float(j * TileSize), -60.0);
			SpawnPosition2 = FVector(float(((MapMaxX - 1) - i) * TileSize), float(((MapMaxY - 1) - j) * TileSize), -60.0);

			TileBuffer1 = SpawnFloor(SpawnPosition1); 
			TileBuffer2 = SpawnFloor(SpawnPosition2);

			if (BlockChance / 2.0 > FMath::FRandRange(0.0, 1.0)) 
			{
				CombatGrid->AddAtCoordinates(i, j, ETileState::TS_Obstructed, TileBuffer1);
				CombatGrid->AddAtCoordinates(((MapMaxY - 1) - j), ((MapMaxX - 1) - i), ETileState::TS_Obstructed, TileBuffer2);
			}
			else 
			{
				CombatGrid->AddAtCoordinates(i, j, ETileState::TS_Empty, TileBuffer1);
				CombatGrid->AddAtCoordinates(((MapMaxY - 1) - j), ((MapMaxX - 1) - i), ETileState::TS_Empty, TileBuffer2);
			}
		}
	}
}

void AMapGenerator::SetPlayerTroops()
{
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			switch (i+j) 
			{
				case 0: case 1: /* Spawn Champion */ 
					// Create Champion
					CombatGrid->SetSpawnPoint(FPosition(i, j));
					CombatGrid->SetSpawnPoint(FPosition((MapMaxX - 1) - i, (MapMaxY - 1) - j));
					break;
				case 2: case 3: /* Spawn Regular */ 
					// Create Pawn
					CombatGrid->SetSpawnPoint(FPosition(i, j));
					CombatGrid->SetSpawnPoint(FPosition((MapMaxX - 1) - i, (MapMaxY - 1) - j));
					break;
				default:
					CombatGrid->FreeCoordinate(FPosition(i, j));
					CombatGrid->FreeCoordinate(FPosition((MapMaxX - 1) - i, (MapMaxY - 1) - j));
					break;
			}
		}
	}
}

void AMapGenerator::FixGround()
{
	// Find a path in the grid
	for (int32 i = 2; i < FMath::Min(MapMaxX, MapMaxY); i++) 
	{
		CombatGrid->FreeCoordinate(FPosition(i, i));

		CombatGrid->FreeCoordinate(FPosition(i + 1, i));
		CombatGrid->FreeCoordinate(FPosition(i - 1, i));

		CombatGrid->FreeCoordinate(FPosition(i, i + 1));
		CombatGrid->FreeCoordinate(FPosition(i, i - 1));
	}
}

void AMapGenerator::GenerateGround()
{
	for (const auto& Position : CombatGrid->GetObstructedPositions())
	{
		ObstructingStructure->AddInstance((FTransform(FVector(Position.Row * TileSize, Position.Column * TileSize, -10.0))));
	}
}


void AMapGenerator::FindGrid()
{
	TArray<AActor*> Grids;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), Grids);

	verify(Grids.Num() > 0);

	CombatGrid = Cast<AGrid>(Grids[0]);

}

void AMapGenerator::FindTroopManager()
{
	TArray<AActor*> TroopManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATroopManager::StaticClass(), TroopManagers);

	verify(TroopManagers.Num() > 0);

	TroopManager = Cast<ATroopManager>(TroopManagers[0]);

}

void AMapGenerator::FindGameMode()
{
	GameMode = Cast<AThisisNotXcomGameMode>(GetWorld()->GetAuthGameMode());

	verify(GameMode.Get() != nullptr);

}

void AMapGenerator::SpawnTeamLeaders()
{
	TeamManager = GetWorld()->SpawnActor<ATeamManager>();

	/* Doesn't work, don't know why
	for (uint8 TeamCounter = 0; TeamCounter < TeamManager->NumberOfTeams; TeamCounter++)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("Spawning player %d"), TeamCounter + 1	));

		GameMode->RegisterTeam
			(TeamManager->CreateTeam(FVector(MapMaxX * TileSize, MapMaxY * TileSize, 100.0f) * TeamCounter,
			(TeamCounter % 2) ? FRotator(0.0f, 180.0f, 0.0f) : FRotator()));
	}
	*/
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Spawning player 1"));

	GameMode->RegisterTeam
		(TeamManager->CreateTeam(FVector(-100.0f, -100.0f, 1000.0f), FRotator(-60.0f, 40.0f, 0.0f)));


	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Spawning player 2"));

	GameMode->RegisterTeam(TeamManager->CreateTeam
		(FVector((MapMaxX + 1) * TileSize, (MapMaxY + 1) * TileSize, 1000.0f), FRotator(-60.0f, 140.0f, 0.0f)));
}

void AMapGenerator::SpawnPlayerTroops()
{
	ATrooper* TrooperBuffer;
	FRotator Team2TroopRotation = { 0.0f, 180.f, 0.0f };

	for (uint8 I = 0; I < 3; I++)
	{
		for (uint8 J = 0; J < 3; J++)
		{
			switch (I + J)
			{
				case 0: case 1: /* Generate Champions */
					// Create and tweak Champions
					TrooperBuffer = Cast<ATrooper>(TroopManager->SpawnChampionAt(FVector(I * TileSize, J * TileSize, 0)));
					if (TrooperBuffer)
					{
						TrooperBuffer->Team = GameMode->GetTeamAt(0);
						CombatGrid->AddOcupantAt(TrooperBuffer, FPosition(I, J));
					}

					TrooperBuffer = Cast<ATrooper>(TroopManager->SpawnChampionAt(FVector(((MapMaxX - 1) - I) * TileSize, ((MapMaxY - 1) - J) * TileSize, 0)));
					if (TrooperBuffer)
					{
						TrooperBuffer->SetActorRotation(Team2TroopRotation);
						TrooperBuffer->Facing = EDirectionEnum::DE_Backward;
						TrooperBuffer->Team = GameMode->GetTeamAt(1);
						CombatGrid->AddOcupantAt(TrooperBuffer, FPosition(((MapMaxX - 1) - I), ((MapMaxY - 1) - J)));
					}
					break;

				case 2: case 3: /* Spawn Regular */
					// Create and tweak Regulars
					TrooperBuffer = (ATrooper*)TroopManager->SpawnPawnAt(FVector(I * TileSize, J * TileSize, 0));
					if (TrooperBuffer)
					{
						TrooperBuffer->Team = GameMode->GetTeamAt(0);
						CombatGrid->AddOcupantAt(TrooperBuffer, FPosition(I, J));
					}
					
					TrooperBuffer = (ATrooper*)TroopManager->SpawnPawnAt(FVector(((MapMaxX - 1) - I) * TileSize, ((MapMaxY - 1) - J) * TileSize, 0));
					if (TrooperBuffer)
					{
						TrooperBuffer->SetActorRotation(Team2TroopRotation);
						TrooperBuffer->Facing = EDirectionEnum::DE_Backward;
						TrooperBuffer->Team = GameMode->GetTeamAt(1);
						CombatGrid->AddOcupantAt(TrooperBuffer, FPosition(((MapMaxX - 1) - I), ((MapMaxY - 1) - J)));
					}
					break;

				default: 
					break;
			}
		}
	}

}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	AActor::BeginPlay();

	FindGrid();

	FindTroopManager();

	FindGameMode();

	SpawnTeamLeaders();

	PreGenerateGround();

	SetPlayerTroops();

	if (!AStar(CombatGrid.Get(), MapMaxX, MapMaxY).isPossiblePathExisting(FPosition(0, 0)))
	{
		FixGround();
	}

	GenerateGround();

	SpawnPlayerTroops();

	Cast<AThisisNotXcomGameMode>(GetWorld()->GetAuthGameMode())->InitFinished();
}

// Called every frame
void AMapGenerator::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

ATile* AMapGenerator::SpawnFloor(const FVector& Position)
{
	return (ATile*) GetWorld()->SpawnActor(FloorClass, &Position);
}