// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamLeader.h"

void UTeamLeader::SpawnChampionAt(const FPosition& Position)
{
	Champions.Add(NewObject<AChampion>());
	CombatGrid->OccupiedBy(Position, Champions.Last().Get());
}

void UTeamLeader::SpawnPawnAt(const FPosition& Position)
{
	Pawns.Add(NewObject<APawnTrooper>());
	CombatGrid->OccupiedBy(Position, Pawns.Last().Get());
}