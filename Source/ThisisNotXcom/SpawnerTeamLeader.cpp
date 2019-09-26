// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerTeamLeader.h"
#include "Grid.h"

FString GetUnitName(EUnitTypeEnum UnitType)
{
	switch (UnitType)
	{
		case EUnitTypeEnum::TCE_Pawn:	return "Pawn";
		case EUnitTypeEnum::TCE_Ranger: return "Ranger";
		case EUnitTypeEnum::TCE_Cham:	return "Champion";

		default:						return "Error";
	}
}

void ASpawnerTeamLeader::BeginPlay()
{
	for (const auto& Pair : UnitManager->MaxUnits)
	{
		RemainingSpawns.Add(Pair);
	}
}

AUnit* ASpawnerTeamLeader::CreateUnit(EUnitTypeEnum UnitType, const FPosition& Position)
{
	FActorSpawnParameters SpawnParams;
	const FMeshKey	MeshKey			=	FMeshKey(UnitManager->TeamColors[this], UnitType);
	const FVector	UnitLocation	=	{ ObjectiveTile->GetActorLocation().X, ObjectiveTile->GetActorLocation().Y, 0.0f };
	const FRotator	UnitRotator		=	FRotator();
	const FString	UnitName		=	PlayerName + GetUnitName(UnitType) + "_" + FString::FromInt(RemainingSpawns[UnitType]);
	SpawnParams.Name				=	FName(*UnitName);

	AUnit*			SpawnedUnit		=	Cast<AUnit>(GetWorld()->SpawnActor(UnitManager->UnitClasses[UnitType], &UnitLocation, &UnitRotator, SpawnParams));

	SpawnedUnit->Team				=	this;
	SpawnedUnit->Location			=	ObjectiveTile.Get();
	SpawnedUnit->Location->State	=	ETileState::TS_Occupied;
	SpawnedUnit->Location->Ocupant	=	SpawnedUnit;

	SpawnedUnit->MeshComponent->SetSkeletalMesh(UnitManager->TeamMeshes[MeshKey]);

	return SpawnedUnit;
}

void ASpawnerTeamLeader::SpawnUnit(EUnitTypeEnum UnitType)
{
	const FPosition	UnitPosition = CombatGrid->GetPosition(ObjectiveTile.Get());

	if	(RemainingSpawns[UnitType] > 0						&&	ObjectiveTile != nullptr && 
		(UnitPosition.Row + UnitPosition.Column + 1) != 16	&&	ObjectiveTile->State == ETileState::TS_Empty)
	{
		Army.Add(CreateUnit(UnitType, UnitPosition));

		if ((RemainingSpawns[UnitType]--) <= 0)
		{
			OnNoSpawnsLeftover.Broadcast(UnitType);
		}
		
		AfterUnitSpawn.Broadcast(ObjectiveTile.Get(), UnitType);
	}
}

void ASpawnerTeamLeader::StopSpawning()
{
	OnEndOfSpawns.Broadcast(this);
}