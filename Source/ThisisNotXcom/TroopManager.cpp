// Fill out your copyright notice in the Description page of Project Settings.

#include "TroopManager.h"
#include "Engine.h"
#include "Engine/Engine.h"
#include "EngineGlobals.h"

UTroopManager::UTroopManager()
{

}

void UTroopManager::SpawnPawnAt(const FVector& Position)
{
	GetWorld()->SpawnActor(PawnClass, &Position);
}