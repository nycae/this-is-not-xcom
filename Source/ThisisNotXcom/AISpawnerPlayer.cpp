// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawnerPlayer.h"

void AAISpawnerPlayer::SpawnUnit(const ATile* Tile, EUnitTypeEnum UnitType)
{
	FTimerHandle	UnusedHandler;
	FTimerDelegate	TimerDelegate;
	const FPosition	EnemyPosition	=	Grid->GetPosition(Tile);
	const FPosition	SpawnPosition	=	FPosition(MapGenerator->SideSizeY - 1 - EnemyPosition.Column, MapGenerator->SideSizeX - 1 - EnemyPosition.Row);

	TeamLeader->ObjectiveTile = Grid->At(SpawnPosition);
	TeamLeader->SpawnUnit(UnitType);
	TeamLeader->ObjectiveTile = nullptr;
}