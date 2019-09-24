// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPlayer.h"
#include "AStar.h"
#include "GridPathfinder.h"


constexpr bool bAreAdjacent(const FPosition& Begin, const FPosition& End)
	{ return (Begin.Row + Begin.Column) - (End.Row + Begin.Column) == 1; }

constexpr int32 AAIPlayer::Offset(const FPosition& Begin, const FPosition& Target)
	{ return FMath::Abs(Begin.Row - Target.Row) + FMath::Abs(Begin.Column - Target.Column); }

AAIPlayer::AAIPlayer() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
}

void AAIPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AAIPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldIAttentATurn)
	{
		TimerForDelayedExecution(Team->Army);
		bShouldIAttentATurn = false;
	}

}

void AAIPlayer::PlayTurn()
{
	bShouldIAttentATurn = true;
}

void AAIPlayer::TimerForDelayedExecution(TArray<AUnit*> Units)
{
	FTimerHandle UnusedHandle;
	FTimerDelegate BindedFunction;

	GoForTheWeakest(Units.Pop());

	if (Units.Num())
	{
		BindedFunction.BindUFunction(this, FName("TimerForDelayedExecution"), Units);
		GetWorldTimerManager().SetTimer(UnusedHandle, BindedFunction, 1.0f, false, 1.0f);
	}
	else
	{
		BindedFunction.BindUFunction(Team, FName("FinishTurn"), Units);
		GetWorldTimerManager().SetTimer(UnusedHandle, BindedFunction, 1.0f, false, 8.0f);
	}
	
}

void AAIPlayer::GoForTheWeakest(AUnit* ArgUnit)
{
	FTimerHandle UnusedHandle;

	Unit			=	ArgUnit;
	UnitPosition	=	Grid->GetPosition(Unit->Location);

	FindNewObjective();
	
	while (bHasEnemiesNearby() && Unit->Energy >= Unit->AttackCost)
	{
		AttackWeakestEnemyNearby();
	}
	
	if (Unit->Energy >= Unit->MovementCost)
	{
		MoveTowardsObjective();
	}
}

TArray<FPosition> AAIPlayer::GetEnemyPositions()
{
	TArray<FPosition> UnitPositions = Grid->GetOccupiedPositions();

	for (const auto& Unit : Team->Army)
	{
		UnitPositions.Remove(Grid->GetPosition(Unit->Location));
	}

	return UnitPositions;
}

TArray<FPosition> AAIPlayer::GetNearbyPositions(const FPosition& Position)
{
	const TArray<FPosition> Candidates =
	{
		FPosition(Position.Row + 1, Position.Column),
		FPosition(Position.Row - 1, Position.Column),
		FPosition(Position.Row, Position.Column + 1),
		FPosition(Position.Row, Position.Column - 1)
	};

	TArray<FPosition> Finales;

	for (const auto& Candidate : Candidates)
	{
		if (Grid->Tiles.Contains(Candidate))
		{
			Finales.Add(Candidate);
		}
	}

	return Finales;
}

FPosition AAIPlayer::GetRegroupPosition()
{
	int32 CurrentOffset = 0;

	for (const auto& AllyUnit : Team->Army)
	{
		if (AllyUnit != Unit)
		{
			for (const auto& NearbyPosition : GetNearbyPositions(UnitPosition))
			{
				if (AStar(Grid, MapMaxX, MapMaxY).isPossiblePathExisting(UnitPosition, NearbyPosition))
				{
					return NearbyPosition;
				}
			}
		}
	}

	return UnitPosition;
}

FPosition AAIPlayer::GetPositionCloseToObjective()
{
	int32 CurrentOffset;
	int32 IndexMax;
	FPosition FinalPosition;

	int32 MaxOffset = 0;
	TArray<FPosition> PossiblePositions = GetNearbyPositions(TargetPosition);

	for (int32 Depth = 1; Depth < Unit->MaxAttackDepth; Depth++)
	{
		IndexMax = PossiblePositions.Num();
		
		for (int32 Index = 0; Index < IndexMax; Index++)
		{
			for (const auto& Position : GetNearbyPositions(PossiblePositions[Index]))
			{
				PossiblePositions.AddUnique(Position);
			}
		}
	}

	for (const auto& Position : PossiblePositions)
	{
		if (Grid->At(Position)->State == ETileState::TS_Empty
			&&
			AStar(Grid, MapMaxX, MapMaxX).isPossiblePathExisting(UnitPosition, Position))
		{
			CurrentOffset = Offset(UnitPosition, Position);

			if (CurrentOffset > MaxOffset
				&&
				MaxOffset <= Unit->MaxAttackDepth)
			{
				MaxOffset = CurrentOffset;
				FinalPosition = Position;
			}
		}
	}
	
	return FinalPosition;
}

FPosition AAIPlayer::GetPositionFromPath(const TArray<EDirectionEnum>& Path)
{
	FPosition FinalPosition = Grid->GetPosition(Unit->Location);

	for (const auto& Direction : Path)
	{
		switch (Direction)
		{
			case EDirectionEnum::DE_Forward:
				FinalPosition.Row++;
				break;

			case EDirectionEnum::DE_Backward:
				FinalPosition.Row--;
				break;
			
			case EDirectionEnum::DE_Right:
				FinalPosition.Column++;
				break;

			case EDirectionEnum::DE_Left:
				FinalPosition.Column--;
				break;
		}
	}

	return FinalPosition;
}

void AAIPlayer::AttackWeakestEnemyNearby()
{
	FPosition FinalPosition;
	int32 MinHealth = TNumericLimits<int32>::Max();

	for (const auto& EnemyPosition : EnemyPositions)
	{
		if (Offset(UnitPosition, EnemyPosition) <= Unit->MaxAttackDepth)
		{
			const AUnit* Enemy = Grid->At(EnemyPosition)->Ocupant;

			if (Enemy->Health < MinHealth)
			{
				FinalPosition = EnemyPosition;
			}
		}
	}

	Attack(FinalPosition);
}

void AAIPlayer::Attack(const FPosition& Position)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Unit at [%d, %d] is attacking [%d, %d]"),
		UnitPosition.Row, UnitPosition.Column, Position.Row, Position.Column));
	while (	Grid->At(Position)->Ocupant != nullptr
			&&
			Unit->Energy >= Unit->AttackCost)
	{
		Unit->Attack(Grid->At(Position));
	}

	if (Grid->At(Position)->Ocupant == nullptr)
	{
		FindNewObjective();
	}
}

void AAIPlayer::MoveTowards(const FPosition& Position)
{
	FPosition FinalPosition;

	const int32 MaxDepth		=	(Unit->Energy / Unit->MovementCost);
	TArray<EDirectionEnum> Path =	GridPathfinder::GetPath(Unit->Location, Grid->At(Position), TNumericLimits<int32>::Max(), Grid);

	while (Path.Num() > MaxDepth)
	{
		Path.RemoveAt(Path.Num() - 1);
	}

	FinalPosition = GetPositionFromPath(Path);
	UnitPosition = FinalPosition;
	Unit->MoveTo(Grid->At(FinalPosition));
}

void AAIPlayer::MoveTowardsObjective()
{
	if (bCanReachObjective())
	{
		MoveTowards(GetPositionCloseToObjective());
	}
	else
	{
		MoveTowards(GetRegroupPosition());
	}
}

void AAIPlayer::FindNewObjective()
{
	EnemyPositions = GetEnemyPositions();

	int32 MinHealth = TNumericLimits<int32>::Max();

	for (const auto& Position : EnemyPositions)
	{
		if (Grid->At(Position)->Ocupant->Health < MinHealth)
		{
			TargetPosition = Position;
		}
	}
}

bool AAIPlayer::bCanReachObjective()
{
	for (const auto& NearbyPosition : GetNearbyPositions(TargetPosition))
	{
		if (AStar(Grid, MapMaxX, MapMaxY).isPossiblePathExisting(UnitPosition, NearbyPosition))
		{
			return true;
		}
	}

	return false;
}

bool AAIPlayer::bHasEnemiesNearby()
{
	for (const auto& EnemyPosition : EnemyPositions)
	{
		if (Offset(UnitPosition, EnemyPosition) <= Unit->MaxAttackDepth)
		{
			return true;
		}
	}

	return false;
}