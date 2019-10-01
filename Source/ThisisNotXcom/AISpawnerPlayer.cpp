// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawnerPlayer.h"

void AAISpawnerPlayer::SpawnUnit(const ATile* Tile, EUnitTypeEnum UnitType)
{
	FTimerHandle	UnusedHandler;
	FTimerDelegate	TimerDelegate;
	const FPosition	UnitPosition	=	Grid->GetPosition(Tile);
	const FPosition	SpawnPosition	=	FPosition(MapGenerator->SideSizeY - 1 - UnitPosition.Column, MapGenerator->SideSizeX - 1 - UnitPosition.Row);

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, FString::Printf(TEXT("Positions: [%d, %d]: [%d, %d]"), UnitPosition.Row, UnitPosition.Column, SpawnPosition.Row, SpawnPosition.Column));
	
	TeamLeader->ObjectiveTile = Grid->At(SpawnPosition);
	TeamLeader->SpawnUnit(UnitType);
	TeamLeader->ObjectiveTile = nullptr;
}