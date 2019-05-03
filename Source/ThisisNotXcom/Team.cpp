// Fill out your copyright notice in the Description page of Project Settings.

#include "Team.h"
#include "Engine.h"

UTeam::UTeam()
{

}

void UTeam::AddChampion(AChampion* Champion)
{
	if (Champions.Num() < 3)
	{
		Champions.Add(Champion);
	}
}

void UTeam::AddPawn(APawnTrooper* Pawn)
{
	if (Pawns.Num() < 5)
	{
		Pawns.Add(Pawn);
	}
}