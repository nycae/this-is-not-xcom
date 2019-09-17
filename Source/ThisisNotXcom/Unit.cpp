// Fill out your copyright notice in the Description page of Project Settings.

#include "Unit.h"
#include "Direction.h"
#include "TeamLeader.h"
#include "Engine/World.h"
#include "GridPathfinder.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


AUnit::AUnit() 
	: Super()
{
 	PrimaryActorTick.bCanEverTick = false;
}

FVector AUnit::GetVectorFromDirection(EDirectionEnum Direction, int32 TileSize) const
{
	FVector FinalPosition = GetActorLocation();

	switch (Direction)
	{
		case EDirectionEnum::DE_Forward:
			FinalPosition.X += TileSize;
			break;

		case EDirectionEnum::DE_Backward:
			FinalPosition.X -= TileSize;
			break;

		case EDirectionEnum::DE_Right:
			FinalPosition.Y += TileSize;
			break;

		case EDirectionEnum::DE_Left:
			FinalPosition.Y -= TileSize;
			break;

	}

	return FinalPosition;
}

FRotator AUnit::GetRotationByDirection(EDirectionEnum Direction) const
{
	switch (Direction)
	{
		case EDirectionEnum::DE_Forward:
			return FRotator(0.0f, 0.0f, 0.0f);
			break;

		case EDirectionEnum::DE_Backward:
			return FRotator(0.0f, 180.0f, 0.0f);
			break;

		case EDirectionEnum::DE_Left:
			return FRotator(0.0f, -90.0f, 0.0f);
			break;

		case EDirectionEnum::DE_Right:
			return FRotator(0.0f, 90.0f, 0.0f);
			break;

		case EDirectionEnum::DE_FwdLeft:
			return FRotator(0.0f, 45.0f, 0.0f);
			break;

		case EDirectionEnum::DE_FwdRight:
			return FRotator(0.0f, -45.0f, 0.0f);
			break;

		case EDirectionEnum::DE_BwdLeft:
			return FRotator(0.0f, -135.0f, 0.0f);
			break;

		case EDirectionEnum::DE_BwdRight:
			return FRotator(0.0f, 135.0f, 0.0f);
			break;
	}

	return FRotator();
}

float AUnit::GetHealthPercentage() const
{
	return ((float)Health) / MaxHealth;
}

float AUnit::GetEnergyPercentage() const
{
	return ((float)Energy) / MaxEnergy;
}

int AUnit::OnAttack(AUnit* Attacker)
{
	Health -= Attacker->AttackDamage;
	if (Health <= 0)
	{
		OnDeath.Broadcast(this, Team);
	}

	PlayDamagedAnimation();

	return Health;
}

int AUnit::GetMaxMoveDepth() const
{
	return Energy / MovementCost;
}

void AUnit::MoveTo(ATile* Tile)
{
	if (Tile->State != ETileState::TS_Empty)
	{
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), FoundActors);
	
	TArray<EDirectionEnum> MovementList = GridPathfinder::GetPath(Location, Tile, GetMaxMoveDepth(), Cast<AGrid>(FoundActors[0]));

	if (MovementList.Num() > 0)
	{
		Team->CancelSelection();
		PlayMovementAnimation(MovementList);

		Location->State = ETileState::TS_Empty;
		Location->Ocupant = nullptr;

		Location = Tile;
		Location->State = ETileState::TS_Occupied;
		Location->Ocupant = this;
	}

	Energy -= MovementList.Num() * MovementCost;
}

void AUnit::Attack(ATile* Tile)
{
	if (Tile->State != ETileState::TS_Occupied && Energy < AttackCost)
	{
		return;
	}

	TArray<AActor*> FoundActors;
	FPosition MyPosition, TargetPosition;
	AGrid* Grid;
	int32 HeightOffset, WidthOffset;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), FoundActors);
	Grid = Cast<AGrid>(FoundActors[0]);
	
	MyPosition = Grid->GetPosition(Location);
	TargetPosition = Grid->GetPosition(Tile);
	HeightOffset = MyPosition.Row - TargetPosition.Row;
	WidthOffset = MyPosition.Column - TargetPosition.Column;

	if (HeightOffset + WidthOffset <= MaxAttackDepth)
	{
		Team->CancelSelection();

		PlayAttackAnimation(GetDirectionByOffset(HeightOffset, WidthOffset));

		Tile->Ocupant->OnAttack(this);
	}
}

EDirectionEnum AUnit::GetDirectionByOffset(int32 HeightOffset, int32 WidthOffset) const
{
	if (HeightOffset == 0 && WidthOffset < 0)
	{
		return EDirectionEnum::DE_Right;
	} 

	if (HeightOffset == 0 && WidthOffset > 0)
	{
		return EDirectionEnum::DE_Left;
	} 

	if (HeightOffset < 0 && WidthOffset == 0)
	{
		return EDirectionEnum::DE_Forward;
	}

	if (HeightOffset > 0 && WidthOffset == 0)
	{
		return EDirectionEnum::DE_Backward;
	}

	if (HeightOffset < 0 && WidthOffset < 0)
	{
		return EDirectionEnum::DE_FwdRight;
	}

	if (HeightOffset < 0 && WidthOffset > 0)
	{
		return EDirectionEnum::DE_FwdLeft;
	}

	if (HeightOffset > 0 && WidthOffset < 0)
	{
		return EDirectionEnum::DE_BwdRight;
	}

	if (HeightOffset > 0 && WidthOffset > 0)
	{
		return EDirectionEnum::DE_BwdLeft;
	}

	return EDirectionEnum::DE_MAX;
}