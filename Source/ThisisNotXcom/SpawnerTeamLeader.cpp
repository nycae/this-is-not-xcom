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

void ASpawnerTeamLeader::SpawnUnit(EUnitTypeEnum UnitType)
{
	if (RemainingSpawns[UnitType] > 0 && ObjectiveTile != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		const FMeshKey	MeshKey			=	FMeshKey(UnitManager->TeamColors[this], UnitType);
		const FVector	UnitLocation	=	{ObjectiveTile->GetActorLocation().X, ObjectiveTile->GetActorLocation().Y, 0.0f};
		const FRotator	UnitRotator		=	FRotator();
		const FString	UnitName		=	GetUnitName(UnitType) + "_" + FString::FromInt(RemainingSpawns[UnitType]);
		const FPosition	UnitPosition	=	CombatGrid->GetPosition(ObjectiveTile.Get());
		SpawnParams.Name				=	FName(*UnitName);

		AUnit*			SpawnedUnit		=	Cast<AUnit>(GetWorld()->SpawnActor(UnitManager->UnitClasses[UnitType], &UnitLocation, &UnitRotator, SpawnParams));
		
		SpawnedUnit->Team				=	this;
		SpawnedUnit->Location			=	ObjectiveTile.Get();
		SpawnedUnit->Location->State	=	ETileState::TS_Occupied;
		SpawnedUnit->Location->Ocupant	=	SpawnedUnit;

		SpawnedUnit->MeshComponent->SetSkeletalMesh(UnitManager->TeamMeshes[MeshKey]);
		
		Army.Add(SpawnedUnit);
		AfterUnitSpawn.Broadcast(this, SpawnedUnit, UnitType);

		if ( (RemainingSpawns[UnitType]--) <= 0)
		{
			OnNoSpawnsLeftover.Broadcast(UnitType);
		}
	}
}

void ASpawnerTeamLeader::StopSpawning()
{
	OnEndOfSpawns.Broadcast(this);
}