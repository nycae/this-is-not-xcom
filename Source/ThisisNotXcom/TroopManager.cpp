// Fill out your copyright notice in the Description page of Project Settings.

#include "TroopManager.h"
#include "Engine.h"
#include "Engine/Engine.h"
#include "EngineGlobals.h"

ATroopManager::ATroopManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATroopManager::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}

void ATroopManager::BeginPlay()
{
	AActor::BeginPlay();
}

AActor* ATroopManager::SpawnPawnAt(const FVector& Position)
{
	if (!PawnClass) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "no pawn class");
		return nullptr;
	}
	else
	{
		return GetWorld()->SpawnActor(PawnClass, &Position);
	}
}

AActor* ATroopManager::SpawnChampionAt(const FVector& Position)
{
	if (!PawnClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "no pawn class");
		return nullptr;
	}
	else
	{
		return GetWorld()->SpawnActor(ChampionClass, &Position);
	}
}